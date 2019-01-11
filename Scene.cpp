#include "Scene.h"



Scene::Scene() {
}

void Scene::addMesh(std::string path) {
	Mesh* toAdd = Mesh::load(path);
	this->addMesh(toAdd);
}

void Scene::addMesh(Mesh* mesh) {
	this->meshes.push_back(mesh);
}

void Scene::draw() {
	for (auto mesh : meshes) {
		mesh->draw();
	}
}

GLuint Scene::size() {
	GLuint size = 0;
	for (auto mesh : this->meshes)
		size += mesh->size();
	return size;
}

Face Scene::getFace(GLuint index) {
	GLuint iterator = index;
	for (auto mesh : this->meshes) {
		if (iterator > mesh->size()) {
			iterator -= mesh->size();
		}
		else {
			return mesh->getFace(iterator);
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

GLuint SceneIterator::next() {
	if (!this->end())
		this->index += 3;
	return this->index;
}

Face SceneIterator::get() {
	return this->scene->getFace(index);
}

GLuint SceneIterator::faceIndex() {
	return this->index / 3;
}

bool SceneIterator::end() {
	return this->index >= this->top;
}
