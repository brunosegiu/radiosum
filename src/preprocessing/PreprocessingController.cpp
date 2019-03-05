#include "preprocessing/PreprocessingController.h"

#include <SparseCholesky> 

#include "display/Window.h"
#include "common/ConfigurationManager.h"
#include "common/geometry/Face.h"
#include "common/Logger.h"
#include "common/Timer.h"

PreprocessingController::PreprocessingController(Scene* scene) {
	this->scene = scene;

	this->renderer = new IDRenderer(scene);

	this->reducer = new ComputeShader("computeRow.comp");
	this->row = new RowBuffer(scene->size() + 1); // Padding for 0 (void)
	std::string widthstr = ConfigurationManager::get("INTERNAL_WIDTH");
	this->instances = std::stoi(widthstr);

	this->pixelCount = GLfloat((this->instances * this->instances) * 3);

	this->iterator = new SceneIterator(scene);
}

GLuint PreprocessingController::runStep() {
	GLuint index = this->iterator->faceIndex();
	if (!this->iterator->end()) {
		Timer stepTimer = Timer();
		// Get camera configuration and prepare for render
		{
			Face face = this->iterator->get();
			glm::vec3 origin, normal;
			glm::vec4 plane = face.getPlane();
			origin = face.getBarycenter();
			normal = face.getNormal();
			Camera faceCamera = Camera(1.0f, 90.0f, 0.5f, 5000.0f, origin, normal);
			this->renderer->setCamera(&faceCamera);
			this->renderer->setClipPlane(plane);
		}


		// Render hemicube and read results
		{
			this->renderer->render();
			this->renderer->read();
		}

		// Process hemicube and compute row
		{
			std::vector<GLuint> faceFactors = this->getMatrixRow(iterator->faceIndex());
			this->workers.push_back(std::thread(&PreprocessingController::processRow, this, faceFactors, iterator->faceIndex()));
		}
		Logger::log("RenderRow", std::to_string(stepTimer.get()) + "s");
		iterator->nextFace();
	}
	return index;
}

void PreprocessingController::processRow(std::vector<GLuint> faceFactors, GLuint faceIndex) {
	GLuint iIndex = faceIndex;
	for (GLuint jIndex = 0; jIndex < faceFactors.size() - 1; jIndex++) {
		float seenFaces = float(faceFactors[jIndex + 1]);
		tripletsLock.lock();
		if (seenFaces > 0.0f) {
			GLfloat entry = -(seenFaces / this->pixelCount) * (scene->getReflactance(iIndex));
			triplets.push_back(Eigen::Triplet<GLfloat>(iIndex, jIndex, entry));
		}
		else if (iIndex == jIndex) {
			triplets.push_back(Eigen::Triplet<GLfloat>(iIndex, jIndex, 1.0f));
		}
		tripletsLock.unlock();
	}
}

std::vector<GLfloat> PreprocessingController::computeRadiosity() {
	Logger::log("Computing radiosity for " + std::to_string(int(scene->size())) + " faces");
	for (GLuint i = 0; i < this->workers.size(); i++) {
		workers[i].join();
	}
	Eigen::SparseMatrix<GLfloat> matrix = Eigen::SparseMatrix<GLfloat>(scene->size(), scene->size());
	matrix.setFromTriplets(this->triplets.begin(), this->triplets.end());

	if (!this->iterator->end()) {
		throw std::runtime_error("Preprocessing not completed yet, radiosity computation called");
	}
	Eigen::SparseLU<Eigen::SparseMatrix<GLfloat>> solver;
	solver.analyzePattern(matrix);
	solver.factorize(matrix);
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

	std::vector<GLfloat> vectorizedRad;
	vectorizedRad.reserve(radiosity.size());
	Eigen::VectorXf::Map(&vectorizedRad[0], radiosity.size()) = radiosity;

	return vectorizedRad;
}

std::vector<GLuint> PreprocessingController::getMatrixRow(GLuint face) {
	this->row->clean();
	this->row->bind();
	this->reducer->bind();
	this->reducer->run(this->instances, this->instances, 1);
	return this->row->getBuffer();
}

PreprocessingController::~PreprocessingController() {
	delete this->iterator;
	delete this->renderer;
}
