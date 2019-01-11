#include "PreprocessingController.h"

#include "display/Window.h"
#include "common/ConfigurationManager.h"
#include "common/geometry/Face.h"

PreprocessingController::PreprocessingController(Scene* scene) {
	this->scene = scene;

	this->renderer = new IDRenderer(scene);

	this->reducer = new ComputeShader("computeRow.comp");
	this->row = new RowBuffer(scene->size());
	std::string widthstr = ConfigurationManager::get("INTERNAL_WIDTH");
	this->instances = std::stoi(widthstr);

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
			Camera faceCamera = Camera(this->instances, this->instances, 45.0f, 0.5f, 5000.0f, origin, normal);
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
		}

		iterator->next();
	}
}

std::vector<GLuint> PreprocessingController::getMatrixRow(GLuint face) {
	this->row->bind();
	this->reducer->bind();
	this->reducer->run(this->instances, this->instances, 1);
	return this->row->getBuffer();
}

PreprocessingController::~PreprocessingController() {
	delete this->iterator;
	delete this->renderer;
}
