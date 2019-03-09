#include "Engine.h"
#include "display/Picker.h"

Engine::Engine() {
	this->mode = FACES;
	this->camera = new Camera(1.0f, 90.0f, 0.5f, 5000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->scene = new Scene();
	this->preprocessorOutput = true;
	this->lock = std::unique_lock<std::mutex>(mtx);

	this->sceneRenderer = new SceneRenderer(this->camera);
	this->displayer = new DisplayController(this->mode);

	glEnable(GL_DEPTH_TEST);
}

// Main camera

Camera* Engine::getCamera() {
	return this->camera;
}

// Engine output

void Engine::startFrame() {
	this->frameLock.notify_one();
}

void Engine::setMode(RenderMode mode) {
	this->mode = mode;
	this->displayer->setMode(mode);
}

RenderMode Engine::getMode() {
	return this->mode;
}

void Engine::preprocess(bool withOutput) {
	if (this->preprocessor)
		delete this->preprocessor;
	this->preprocessor = new PreprocessingController(this->scene);
	this->setMode(PREPROCESS);
	this->preprocessorOutput = withOutput;
}

void Engine::computeRadiosity() {
	this->preprocessor->computeRadiosity();
}

// Main scene
void Engine::addMesh(std::string path) {
	this->scene->addMesh(path);
}

Scene* Engine::getScene() {
	return this->scene;
}

void Engine::loop() {
	//this->frameLock.wait(lock);
	while (true) {
		if (this->mode == PREPROCESS && this->preprocessor != nullptr) {
			this->preprocessor->runStep();
		}
		else {
			this->sceneRenderer->setScene(scene);
			this->sceneRenderer->render();
			this->sceneRenderer->read();
		}
		this->displayer->render();
		this->outputTexture = this->displayer->read();
		return;
	}
}

GLuint Engine::pick(GLint x, GLint y) {
	Picker picker(this->scene->size(), dynamic_cast<PickingBuffer*>(this->sceneRenderer->getBuffer()));
	return picker.getSelected(x, y);
}

Engine::~Engine() {
	delete this->scene;
	delete this->camera;
	if (this->preprocessor)
		delete this->preprocessor;
	delete this->sceneRenderer;
	delete this->displayer;
}