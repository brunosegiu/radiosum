#pragma once

#include <map>

#include <GL/glew.h>

class VAO {
public:
	VAO();
	void addAttribute(GLsizei size, void* data, GLuint unitsPerVertex, GLenum type, GLuint id, GLenum usage = GL_STATIC_DRAW);
	void updateAttribute(GLsizei size, void* data, GLuint unitsPerVertex, GLenum type, GLuint id);
	void bind();
	void unbind();
	virtual ~VAO();
private:
	GLuint GLVaoId;
	std::map<GLuint, GLuint> GLAttributesIds;
};

