#include "preprocessing/PreprocessingController.h"

#include <SparseCholesky> 

#include "ConfigurationManager.h"
#include "geometry/Face.h"
#include "EngineStore.h"
#include "Timer.h"

#include <iostream>


PreprocessingController::PreprocessingController(Scene* scene) {
	this->scene = scene;

	this->renderer = new IDRenderer(scene);

	this->reducer = new ComputeShader("computeRow.comp");
	this->row = new RowBuffer(scene->size() + 1); // Padding for 0 (void)
	std::string widthstr = ConfigurationManager::get("INTERNAL_WIDTH");
	this->instances = std::stoi(widthstr);
	this->corrector = new HemicubeCorrector(this->instances);

	this->pixelCount = GLfloat((this->instances * this->instances) * 3);

	this->iterator = new SceneIterator(scene);
	this->runStep();

	this->shouldUpdateGeom[RED] = false;
	this->shouldUpdateGeom[GREEN] = false;
	this->shouldUpdateGeom[BLUE] = false;
	this->channelCount = 0;

	this->shouldInterpolate = false;
}

void PreprocessingController::setUpRenderer() {
	// Get camera configuration and prepare for render
	{
		Face face = this->iterator->get();
		glm::vec3 origin, normal, up;
		glm::vec4 plane = face.getPlane();
		origin = face.getBarycenter();
		normal = face.getNormal();
		up = glm::normalize(face.v1 - face.v0);
		origin += normal * 0.01f;
		delete this->renderer->getCamera();
		Camera* faceCamera = new Camera(1.0f, 90.0f, 0.2f, 500.0f, origin, normal, up);
		this->renderer->setCamera(faceCamera);
		this->renderer->setClipPlane(plane);
	}
}

GLuint PreprocessingController::runStep() {
	GLuint index = this->iterator->faceIndex();
	if (index == 0) {
		this->waitForWorkers();
	}
	EngineStore::progress = index / GLfloat(this->scene->size());
	if (!this->iterator->end()) {
		Timer stepTimer = Timer();

		this->renderer->start();
		this->setUpRenderer();
		// Render hemicube and read results
		{
			this->renderer->render();
			this->renderer->read();
		}

		// Process hemicube and compute row
		{
			std::vector<GLfloat> faceFactors = this->getMatrixRow(iterator->faceIndex());
			this->formFactorWorkers.push_back(std::thread(&PreprocessingController::processRow, this, faceFactors, iterator->faceIndex()));
		}
		EngineStore::logger.log("RenderRow", stepTimer.get());
		iterator->nextFace();
	}
	return index;
}

void PreprocessingController::runUnsafe(bool full) {
	while (!this->iterator->end()) {
		this->renderer->start();

		this->setUpRenderer();

		{
			this->renderer->render();
			this->renderer->read();
		}

		{
			std::vector<GLfloat> faceFactors = this->getMatrixRow(iterator->faceIndex());
			this->formFactorWorkers.push_back(std::thread(&PreprocessingController::processRow, this, faceFactors, iterator->faceIndex()));
		}
		iterator->nextFace();
	}
	if (full) {
		//this->computeRadiosity();
	}
}

void PreprocessingController::processRow(std::vector<GLfloat> faceFactors, GLuint faceIndex) {
	GLuint iIndex = faceIndex;
	for (GLuint jIndex = 0; jIndex < faceFactors.size() - 1; jIndex++) {
		GLfloat ff = GLfloat(faceFactors[jIndex + 1]);
		if (iIndex == jIndex) {
			tripletsLock.lock();
			triplets.push_back(Eigen::Triplet<GLfloat>(iIndex, jIndex, 1.0f));
			tripletsLock.unlock();
		}
		else if (ff > 0.0f) {
			tripletsLock.lock();
			triplets.push_back(Eigen::Triplet<GLfloat>(iIndex, jIndex, (1.0f / this->pixelCount) * ff));
			tripletsLock.unlock();
		}
	}
}

void PreprocessingController::crWrapped(Channel channel) {
	EngineStore::logger.log("Computing radiosity for " + std::to_string(int(scene->size())) + " faces");

	if (!this->iterator->end()) {
		throw std::runtime_error("Preprocessing not completed yet, radiosity computation called");
	}

	// Init Eigen Matrix
	Eigen::SparseMatrix<GLfloat> matrix = Eigen::SparseMatrix<GLfloat>(scene->size(), scene->size());
	matrix.setFromTriplets(this->triplets.begin(), this->triplets.end());

	// Multiply reflactance for channel
	std::vector<glm::vec3> reflactance = this->scene->getReflactances();
	for (int k = 0; k < matrix.outerSize(); ++k) {
		for (Eigen::SparseMatrix<GLfloat>::InnerIterator it(matrix, k); it; ++it) {
			it.valueRef() = it.row() == it.col() ? it.value() : -it.value() * reflactance[it.row()][channel];
		}
	}

	EngineStore::radiosityProgress += .1f / GLfloat(this->channelCount);

	// Initialize solver
	Eigen::SparseLU<Eigen::SparseMatrix<GLfloat>> solver;
	solver.analyzePattern(matrix);
	solver.factorize(matrix);

	EngineStore::radiosityProgress += .4f / GLfloat(this->channelCount);

	if (solver.info() != Eigen::Success) {
		throw std::runtime_error("Cannot compute radiosity from form factor matrix");
	}

	// Get emissions
	std::vector<GLfloat> _emissions = this->scene->getEmissions();
	Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> emissions(_emissions.data(), _emissions.size());
	Eigen::VectorXf _radiosity(emissions.size());

	// Compute radiosity
	_radiosity = solver.solve(emissions);
	if (solver.info() != Eigen::Success) {
		throw std::runtime_error("Cannot compute radiosity from form factor matrix");
	}

	EngineStore::radiosityProgress += .4f / GLfloat(this->channelCount);

	// Update geometry
	this->radiosity[channel] = std::vector<GLfloat>(size_t(_radiosity.size()), 0.0f);
	Eigen::VectorXf::Map(&(this->radiosity[channel])[0], _radiosity.size()) = _radiosity;

	this->shouldUpdateGeom[channel] = true;

	EngineStore::radiosityProgress += .1f;
}

void PreprocessingController::computeRadiosity(std::vector<Channel> channels, bool smooth) {
	EngineStore::radiosityProgress = .0f;
	this->waitForWorkers();
	this->shouldInterpolate = smooth;
	this->channelCount = channels.size();
	for (Channel channel : channels) {
		this->radiosityWorkers.push_back(std::thread(&PreprocessingController::crWrapped, this, channel));
	}
}

void PreprocessingController::checkFlags() {
	if (this->channelCount > 0) {
		bool finished = true;
		for (GLuint i = 0; i < this->channelCount; i++) {
			finished = finished && this->shouldUpdateGeom[i];
		}
		if (finished) {
			std::vector<glm::vec3> radiosityVec3;
			radiosityVec3.reserve(this->radiosity[RED].size());
			for (GLuint i = 0; i < this->radiosity[RED].size(); i++) {
				glm::vec3 rad(.0f);
				for (GLuint channel = 0; channel < this->channelCount; channel++) {
					rad[channel] = this->radiosity[channel][i];
				}
				radiosityVec3.push_back(rad);
			}
			this->scene->setRadiosity(radiosityVec3, this->shouldInterpolate);
			this->shouldUpdateGeom[RED] = false;
			this->shouldUpdateGeom[GREEN] = false;
			this->shouldUpdateGeom[BLUE] = false;
			this->channelCount = 0;
		}
	}
}

void PreprocessingController::waitForWorkers() {
	for (auto &worker : formFactorWorkers) {
		if (worker.joinable())
			worker.join();
	}
	for (auto &radiosityWorkers : radiosityWorkers) {
		if (radiosityWorkers.joinable())
			radiosityWorkers.join();
	}
	this->formFactorWorkers.clear();
	this->radiosityWorkers.clear();
}

std::vector<GLfloat> PreprocessingController::getMatrixRow(GLuint face) {
	this->reducer->bind();
	this->row->bind();
	this->row->clean();
	this->corrector->read();
	this->reducer->run(this->instances, this->instances, 1);
	return this->row->getBuffer();
}

PreprocessingController::~PreprocessingController() {
	this->waitForWorkers();
	delete this->iterator;
	delete this->renderer;
}
