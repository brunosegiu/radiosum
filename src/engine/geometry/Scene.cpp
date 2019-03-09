#include "geometry/Scene.h"

#include "EngineStore.h"

Scene::Scene() {
	this->_size = 0;
}

void Scene::draw() {
	for (auto mesh : meshes) {
		mesh->draw();
	}
}

GLuint Scene::size() {
	return this->_size;
}

Face Scene::getFace(GLuint index) {
	GLuint meshIndex = 0;
	Mesh* mesh = this->getMeshWithIndex(index, meshIndex);
	return mesh->getFace(meshIndex);
}

std::vector<GLfloat> Scene::getEmissions() {
	std::vector<GLfloat> emissions;
	for (auto mesh : meshes) {
		std::vector<GLfloat> meshEmissions = mesh->getEmissions();
		emissions.insert(emissions.end(), meshEmissions.begin(), meshEmissions.end());
	}
	return emissions;
}

std::vector<GLfloat> Scene::getReflactances() {
	std::vector<GLfloat> reflactances;
	for (auto mesh : meshes) {
		std::vector<GLfloat> meshReflactances = mesh->getReflactances();
		reflactances.insert(reflactances.end(), meshReflactances.begin(), meshReflactances.end());
	}
	return reflactances;
}

GLfloat Scene::getEmission(GLuint faceIndex) {
	GLuint meshIndex = 0;
	if (_size > 0) {
		Mesh* mesh = this->getMeshWithIndex(faceIndex, meshIndex);
		return mesh->getEmission(meshIndex);
	}
	else {
		return .0f;
	}
}

GLfloat Scene::getReflactance(GLuint faceIndex) {
	GLuint meshIndex = 0;
	if (_size > 0) {
		Mesh* mesh = this->getMeshWithIndex(faceIndex, meshIndex);
		return mesh->getReflactance(meshIndex);
	}
	else {
		return .0f;
	}
}

void Scene::addMesh(std::string path) {
	EngineStore::logger.log("Adding mesh from path: " + path);
	Mesh* toAdd = Mesh::load(path);
	this->addMesh(toAdd);
}

void Scene::addMesh(Mesh* mesh) {
	_size += mesh->size();
	this->meshes.push_back(mesh);
}

void Scene::setRadiosity(std::vector<GLfloat> &radiosity) {
	GLuint currentIndex = 0;
	for (auto &mesh : this->meshes) {
		mesh->setRadiosity(std::vector<GLfloat>(radiosity.begin() + currentIndex, radiosity.begin() + currentIndex + mesh->size()));
	}
}

void Scene::setEmission(GLuint faceIndex, GLfloat emission) {
	if (_size > 0) {
		GLuint meshIndex = 0;
		Mesh* mesh = this->getMeshWithIndex(faceIndex, meshIndex);
		mesh->setEmission(meshIndex, emission);
	}
}

void Scene::setReflactance(GLuint faceIndex, GLfloat reflactance) {
	if (_size > 0) {
		GLuint meshIndex = 0;
		Mesh* mesh = this->getMeshWithIndex(faceIndex, meshIndex);
		mesh->setReflactance(meshIndex, reflactance);
	}
}

Mesh* Scene::getMeshWithIndex(GLuint faceIndex, GLuint &meshIndex) {
	GLuint iterator = faceIndex;
	meshIndex = faceIndex;
	for (auto &mesh : this->meshes) {
		if (iterator > mesh->size()) {
			iterator -= mesh->size();
			meshIndex = iterator;
		}
		else {
			return mesh;
		}
	}
	throw std::runtime_error("Face index out of range");
}

Scene::~Scene() {
	for (auto mesh : meshes) {
		delete mesh;
	}
}

SceneIterator::SceneIterator(Scene* scene) {
	this->scene = scene;
	this->index = 0;
	this->top = this->scene->size();
}

GLuint SceneIterator::begin() {
	this->index = 0;
	return this->index;
}

GLuint SceneIterator::nextFace() {
	if (!this->end())
		this->index += 1;
	return this->index;
}

GLuint SceneIterator::nextVertex() {
	if (!this->end())
		this->index++;
	return this->index;
}

Face SceneIterator::get() {
	return this->scene->getFace(index);
}

GLuint SceneIterator::faceIndex() {
	return this->index;
}

bool SceneIterator::end() {
	return this->index >= this->top;
}
