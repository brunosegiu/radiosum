#pragma once

#include <map>

#include <GL/glew.h>

class VAO {
public:
	VAO();
	GLuint genBuffer(GLsizei size, void* data, GLenum usage = GL_STATIC_DRAW);
	void addAttribute(GLsizei size, void* data, GLuint unitsPerVertex, GLenum type, GLuint id, GLenum usage = GL_STATIC_DRAW);
	void addAttribute(GLuint bufferId, GLuint unitsPerVertex, GLenum type, GLuint id);
	void updateAttribute(GLintptr offset, GLsizei size, void* data, GLuint id);
	void bind();
	void unbind();
	virtual ~VAO();
private:
	GLuint GLVaoId;
	std::map<GLuint, GLuint> GLAttributesIds;
};

