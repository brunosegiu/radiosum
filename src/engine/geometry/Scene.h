#pragma once

#include "geometry/Mesh.h"
#include "geometry/Face.h"

class Scene {
public:
	Scene();

	void draw(GLuint shaderID);

	// Getters
	// Geometry
	GLuint size();
	Face getFace(GLuint index);

	// Lighting
	std::vector<GLfloat> getEmissions();
	std::vector<GLfloat> getReflactances();
	GLfloat getEmission(GLuint faceIndex);
	GLfloat getReflactance(GLuint faceIndex);

	// Setters
	// Geometry
	void addMesh(std::string path);
	void addMesh(Mesh* mesh);

	// Lighting
	void setEmission(GLuint faceIndex, GLfloat emission);
	void setReflactance(GLuint faceIndex, GLfloat reflactance);
	void setRadiosity(std::vector<GLfloat> &radiosity);

	virtual ~Scene();
private:
	std::vector<Mesh*> meshes;
	Mesh* getMeshWithIndex(GLuint faceIndex, GLuint &meshIndex);
	GLuint _size;
};

class SceneIterator {
public:
	SceneIterator(Scene* scene);

	GLuint begin();
	bool end();

	GLuint nextFace();
	GLuint nextVertex();

	Face get();
	GLuint faceIndex();
private:
	Scene* scene;
	GLuint index, top;
};

