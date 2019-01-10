#pragma once

#include <string>

#include <GL/glew.h>

class ComputeShader {
public:
	ComputeShader(std::string path);
	void bind();
	void unbind();
	void run(GLuint dimX, GLuint dimY, GLuint dimZ);
	GLuint getID();
	virtual ~ComputeShader();
private:
	GLuint GLProgramId;
	void link();
};