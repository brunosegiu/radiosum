#include "geometry/VAO.h"

VAO::VAO() {
	this->GLVaoId = 0;
	glGenVertexArrays(1, &GLVaoId);
}

GLuint VAO::genBuffer(GLsizei size, void* data, GLenum usage) {
	GLuint attrBufferId = 0;
	glGenBuffers(1, &attrBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, attrBufferId);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	return attrBufferId;
}

void VAO::addAttribute(GLsizei size, void* data, GLuint unitsPerVertex, GLenum type, GLuint id, GLenum usage) {
	glBindVertexArray(GLVaoId);
	GLuint attrBufferId = this->genBuffer(size, data, usage);
	glVertexAttribPointer(id, unitsPerVertex, type, GL_FALSE, 0, (void*)0);
	GLAttributesIds[id] = attrBufferId;
}

void VAO::addAttribute(GLuint bufferId, GLuint unitsPerVertex, GLenum type, GLuint id) {
	glBindVertexArray(GLVaoId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glVertexAttribPointer(id, unitsPerVertex, type, GL_FALSE, 0, (void*)0);
	GLAttributesIds[id] = bufferId;
}

void VAO::updateAttribute(GLsizei size, void* data, GLuint unitsPerVertex, GLenum type, GLuint id) {
	glBindVertexArray(GLVaoId);
	glBindBuffer(GL_ARRAY_BUFFER, GLAttributesIds[id]);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(id, unitsPerVertex, type, GL_FALSE, 0, (void*)0);
}

void VAO::bind() {
	glBindVertexArray(GLVaoId);
	for (auto attr : GLAttributesIds) {
		glEnableVertexAttribArray(attr.first);
	}
}

void VAO::unbind() {
	glBindVertexArray(GLVaoId);
	for (auto attr : GLAttributesIds) {
		glDisableVertexAttribArray(attr.first);
	}
}

VAO::~VAO() {
	glBindVertexArray(this->GLVaoId);
	for (auto attr : GLAttributesIds) {
		glDeleteBuffers(1, &attr.second);
	}
	glDeleteVertexArrays(1, &this->GLVaoId);
}
