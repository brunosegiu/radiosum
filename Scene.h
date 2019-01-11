#pragma once

#include "Mesh.h"
#include "Face.h"

class Scene {
public:
	Scene();
	void addMesh(std::string path);
	void addMesh(Mesh* mesh);
	void draw();
	GLuint size();
	Face getFace(GLuint index);
	virtual ~Scene();
private:
	std::vector<Mesh*> meshes;
};

class SceneIterator {
public:
	SceneIterator(Scene* scene);
	GLuint begin();
	GLuint next();
	Face get();
	GLuint faceIndex();
	bool end();
private:
	Scene* scene;
	GLuint index, top;
};

