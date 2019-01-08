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


Scene::~Scene() {
	for (auto mesh : meshes) {
		delete mesh;
	}
}
