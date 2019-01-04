#pragma once

#include "Mesh.h"

class Scene {
public:
	Scene();
	void addMesh(std::string path);
	void addMesh(Mesh* mesh);
	void draw();
	virtual ~Scene();
private:
	std::vector<Mesh*> meshes;
};

