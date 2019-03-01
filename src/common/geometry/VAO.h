#pragma once

#include <vector>

#include <GL/glew.h>

class VAO {
public:
	VAO();
	void addAttribute(GLsizei size, void* data, GLuint unitsPerVertex, GLenum type, GLuint id);
	void bind();
	void unbind();
	virtual ~VAO();
private:
	GLuint GLVaoId;
	std::vector<GLuint> GLAttributesIds;
	std::vector<GLuint> GLCustomAttrIds;
};

