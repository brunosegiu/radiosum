#include "preprocessing/PreprocessingController.h"

#include <SparseCholesky> 

#include "display/Window.h"
#include "common/ConfigurationManager.h"
#include "common/geometry/Face.h"

PreprocessingController::PreprocessingController(Scene* scene) {
	this->scene = scene;

	this->renderer = new IDRenderer(scene);

	this->reducer = new ComputeShader("computeRow.comp");
	this->row = new RowBuffer(scene->size() + 1); // Padding for 0 (void)
	std::string widthstr = ConfigurationManager::get("INTERNAL_WIDTH");
	this->instances = std::stoi(widthstr);

	this->matrix = Eigen::SparseMatrix<GLfloat>(scene->size(), scene->size());
	this->pixelCount = GLfloat((this->instances * this->instances) * 3);

	this->iterator = new SceneIterator(scene);
}

void PreprocessingController::runStep() {
	if (!this->iterator->end()) {

		// Get camera configuration and prepare for render
		{
			Face face = this->iterator->get();
			glm::vec3 origin, normal;
			glm::vec4 plane = face.getPlane();
			origin = face.getBarycenter();
			normal = face.getNormal();
			Camera faceCamera = Camera(1.0f, 45.0f, 0.5f, 5000.0f, origin, normal);
			this->renderer->setCamera(&faceCamera);
			this->renderer->setClipPlane(plane);
		}

		// Render hemicube and read results
		{
			this->renderer->render();
			this->renderer->read();
		}

		// Process hemicube and comute row
		{
			std::vector<GLuint> faceFactors = this->getMatrixRow(iterator->faceIndex());
			GLuint iIndex = iterator->faceIndex();
			for (GLuint jIndex = 0; jIndex < faceFactors.size() - 1; jIndex++) {
				this->matrix.coeffRef(iIndex, jIndex) += float(faceFactors[jIndex + 1]) / this->pixelCount;
			}
			for (Eigen::SparseMatrix<float>::InnerIterator it(this->matrix, iIndex); it; ++it) {
				auto cell = this->matrix.coeffRef(it.row(), it.col());
				if (it.col() == it.row()) {
					cell = 1.0f;
				}
				else {
					cell *= -scene->getReflactance(iIndex);
				}
			}
		}

		iterator->next();
	}
	else {
		this->computeRadiosity();
	}
}

std::vector<float> PreprocessingController::computeRadiosity() {
	if (!this->iterator->end()) {
		throw std::runtime_error("Preprocessing not completed yet, radiosity computation called");
	}
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<GLfloat>> solver;
	solver.compute(this->matrix);
	if (solver.info() != Eigen::Success) {
		throw std::runtime_error("Cannot compute radiosity from form factor matrix");
	}
	std::vector<GLfloat> _emissions = scene->getEmissions();
	Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> emissions(_emissions.data(), _emissions.size());
	Eigen::VectorXf radiosity(emissions.size());
	radiosity = solver.solve(emissions);
	if (solver.info() != Eigen::Success) {
		throw std::runtime_error("Cannot compute radiosity from form factor matrix");
	}
	std::vector<GLfloat> _radiosity(radiosity.data(), radiosity.data() + radiosity.cols()*radiosity.rows());

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
