#pragma once

#include "common/geometry/Mesh.h"
#include "common/geometry/Face.h"

class Scene {
public:
	Scene();
	void addMesh(std::string path);
	void addMesh(Mesh* mesh);
	void draw();
	GLuint size();
	Face getFace(GLuint index);
	std::vector<GLfloat> getEmissions();
	GLfloat getReflactance(GLuint faceIndex);
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

