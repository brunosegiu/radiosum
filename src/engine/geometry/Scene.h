#pragma once

#include "geometry/Mesh.h"
#include "geometry/Face.h"
#include "EngineTypes.h"

class Scene {
public:
	Scene();

	void draw(GLuint shaderID, bool justGeometry = false);

	// Getters
	// Geometry
	GLuint size();
	Face getFace(GLuint index);

	// Lighting
	std::vector<GLfloat> getEmissions();
	std::vector<glm::vec3> getReflactances();
	std::vector<glm::vec3> getRadiosity();
	GLfloat getEmission(GLuint faceIndex);
	glm::vec3 getReflactance(GLuint faceIndex);

	// Setters
	// Geometry
	void addMesh(std::string path);
	void addMesh(Mesh* mesh);

	// Lighting
	void setEmission(GLuint faceIndex, GLfloat emission, bool full = false);
	void setReflactance(GLuint faceIndex, glm::vec3 reflactance, bool full = false);
	void setRadiosity(std::vector<glm::vec3> &radiosity, bool smooth);

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

