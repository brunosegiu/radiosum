#include "preprocessing/PreprocessingController.h"

#include <SparseCholesky> 

#include "ConfigurationManager.h"
#include "geometry/Face.h"
#include "EngineStore.h"
#include "Timer.h"


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
	this->shouldUpdateGeom = false;
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
			this->workers.push_back(std::thread(&PreprocessingController::processRow, this, faceFactors, iterator->faceIndex()));
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
			this->workers.push_back(std::thread(&PreprocessingController::processRow, this, faceFactors, iterator->faceIndex()));
		}
		iterator->nextFace();
	}
	if (full) {
		this->computeRadiosity();
	}
}

void PreprocessingController::processRow(std::vector<GLfloat> faceFactors, GLuint faceIndex) {
	GLuint iIndex = faceIndex;
	GLfloat reflactance = this->scene->getReflactance(faceIndex);
	for (GLuint jIndex = 0; jIndex < faceFactors.size() - 1; jIndex++) {
		GLfloat ff = GLfloat(faceFactors[jIndex + 1]);
		if (iIndex == jIndex) {
			tripletsLock.lock();
			triplets.push_back(Eigen::Triplet<GLfloat>(iIndex, jIndex, 1.0f));
			tripletsLock.unlock();
		}
		else if (ff > 0.0f) {
			tripletsLock.lock();
			triplets.push_back(Eigen::Triplet<GLfloat>(iIndex, jIndex, -reflactance * (1.0f / this->pixelCount) * ff));
			tripletsLock.unlock();
		}
	}
}

void PreprocessingController::crWrapped() {
	EngineStore::logger.log("Computing radiosity for " + std::to_string(int(scene->size())) + " faces");
	for (GLuint i = 0; i < this->workers.size(); i++) {
		workers[i].join();
	}
	this->workers.clear();

	Eigen::SparseMatrix<GLfloat> matrix = Eigen::SparseMatrix<GLfloat>(scene->size(), scene->size());
	matrix.setFromTriplets(this->triplets.begin(), this->triplets.end());

	if (!this->iterator->end()) {
		throw std::runtime_error("Preprocessing not completed yet, radiosity computation called");
	}
	EngineStore::radiosityProgress += .2f;
	Eigen::SparseLU<Eigen::SparseMatrix<GLfloat>> solver;
	solver.analyzePattern(matrix);
	solver.factorize(matrix);
	EngineStore::radiosityProgress += .3f;
	if (solver.info() != Eigen::Success) {
		throw std::runtime_error("Cannot compute radiosity from form factor matrix");
	}

	std::vector<GLfloat> _emissions = this->scene->getEmissions();
	Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> emissions(_emissions.data(), _emissions.size());
	Eigen::VectorXf radiosity(emissions.size());

	radiosity = solver.solve(emissions);
	if (solver.info() != Eigen::Success) {
		throw std::runtime_error("Cannot compute radiosity from form factor matrix");
	}
	EngineStore::radiosityProgress += .4f;
	this->vectorizedRad = std::vector<GLfloat>(size_t(radiosity.size()), 0.0f);
	Eigen::VectorXf::Map(&vectorizedRad[0], radiosity.size()) = radiosity;
	this->shouldUpdateGeom = true;
	EngineStore::radiosityProgress += .1f;
}

void PreprocessingController::computeRadiosity() {
	EngineStore::radiosityProgress = .0f;
	if (this->radiosityThread.joinable())
		this->radiosityThread.join();
	this->radiosityThread = std::thread(&PreprocessingController::crWrapped, this);
}

void PreprocessingController::checkGeometry() {
	if (this->shouldUpdateGeom)
		this->scene->setRadiosity(vectorizedRad);
	this->shouldUpdateGeom = false;
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
	for (GLuint i = 0; i < this->workers.size(); i++) {
		workers[i].join();
	}
	this->radiosityThread.join();
	delete this->iterator;
	delete this->renderer;
}
