#include "Engine.h"

#include "display/Picker.h"
#include "EngineStore.h"
#include <fstream>
#include <sstream>
#include <iostream>

Engine::Engine() {
	this->mode = FACES;
	this->camera = new Camera(1.0f, 90.0f, 0.5f, 5000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->scene = new Scene();

	this->sceneRenderer = new SceneRenderer(this->camera);
	this->displayer = new DisplayController(this->mode);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

// Main camera

Camera* Engine::getCamera() {
	return this->camera;
}

// Engine output

void Engine::setMode(RenderMode mode) {
	if (EngineStore::pipelineStage != FF_LOADING) {
		this->mode = mode;
		this->displayer->setMode(mode);
	}

}

RenderMode Engine::getMode() {
	return this->mode;
}

void Engine::preprocess(bool withOutput) {
	if (this->preprocessor)
		delete this->preprocessor;
	this->preprocessor = new PreprocessingController(this->scene);
	this->setMode(PREPROCESS);
	if (!withOutput)
		this->preprocessor->runUnsafe(false);
}

void Engine::computeRadiosity(std::vector<Channel> channels, bool smooth) {
	std::set<Channel> sChannels;
	for (auto &channel : channels) {
		sChannels.insert(channel);
	}
	this->preprocessor->computeRadiosity(sChannels, smooth);
}

void Engine::setRadiosity(bool smooth) {
	auto rads = this->scene->getRadiosity();
	this->scene->setRadiosity(rads, smooth);
}

// Main scene
void Engine::importGeometry(std::string path) {
	this->scene->addMesh(path);
}

Scene* Engine::getScene() {
	return this->scene;
}

void Engine::step() {
	if (this->preprocessor != nullptr) {
		this->preprocessor->checkFlags();
	}
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

}

GLuint Engine::pick(GLint x, GLint y) {
	Picker picker(this->scene->size(), dynamic_cast<PickingBuffer*>(this->sceneRenderer->getBuffer()));
	return picker.getSelected(x, y);
}

void Engine::resetScene() {
	Scene* old = this->scene;
	this->scene = new Scene();
	delete old;
}

void Engine::exportGeometry(std::string path) {

}

void Engine::exportFFMatrix(std::string path) {

}

void Engine::exportEmission(std::string path) {
	std::ofstream file;
	file.open(path.data());
	auto emission = this->scene->getEmissions();
	for (GLuint index = 0; index < emission.size(); i++) {
		file << index << " " << emission[index] << std::endl;
	}
	file.close();
}

void Engine::exportReflactance(std::string path) {
	std::ofstream file;
	file.open(path.data());
	auto reflactance = this->scene->getReflactances();
	for (GLuint index = 0; index < reflactance.size(); i++) {
		file << index << " " << reflactance[index].x << " " << reflactance[index].y << " " << reflactance[index].z << std::endl;
	}
	file.close();
}

void Engine::exportRadiosity(std::string path) {
	std::ofstream file;
	file.open(path.data());
	auto radiosity = this->scene->getRadiosity();
	for (GLuint index = 0; index < radiosity.size(); i++) {
		file << index << " " << radiosity[index].x << " " << radiosity[index].y << " " << radiosity[index].z << std::endl;
	}
	file.close();
}

void Engine::importFFMatrix(std::string path) {
}

void Engine::importEmission(std::string path) {
	std::ifstream input(path);
	std::string line;
	std::vector<GLfloat> emission(this->scene->size(), .0f);
	while (getline(input, line)) {
		GLuint index = 0;
		GLfloat value = .0f;
		sscanf_s(line.c_str(), "%d %f ", &index, &value);
		if (index < this->scene->size())
			emission[index] = value;
		else
			EngineStore::logger.log("Unable to add emission " + std::to_string(index) + ", out of range.");
	}
	this->scene->setEmission(emission);
}

void Engine::importReflactance(std::string path) {
	std::ifstream input(path);
	std::string line;
	std::vector<glm::vec3> reflactance(this->scene->size(), glm::vec3(.0f));
	while (getline(input, line)) {
		GLuint index = 0;
		glm::vec3 value(.0f);
		sscanf_s(line.c_str(), "%d %f %f %f ", &index, &value.x, &value.y, &value.z);
		if (index < this->scene->size())
			reflactance[index] = value;
		else
			EngineStore::logger.log("Unable to add reflactance " + std::to_string(index) + ", out of range.");
	}
	this->scene->setReflactance(reflactance);
}

void Engine::importRadiosity(std::string path) {

}

Engine::~Engine() {
	delete this->scene;
	delete this->camera;
	if (this->preprocessor)
		delete this->preprocessor;
	delete this->sceneRenderer;
	delete this->displayer;
}