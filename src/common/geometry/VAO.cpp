#include "common/geometry/VAO.h"



VAO::VAO() {
	this->GLVaoId = 0;
	glGenVertexArrays(1, &GLVaoId);
}

void VAO::addAttribute(GLsizei size, void* data, GLuint unitsPerVertex, GLenum type, GLuint id) {
	glBindVertexArray(GLVaoId);
	GLuint attrBufferId = 0;
	glGenBuffers(1, &attrBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, attrBufferId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, attrBufferId);
	glVertexAttribPointer(id, unitsPerVertex, type, GL_FALSE, 0, (void*)0);
	GLAttributesIds.push_back(attrBufferId);
	GLCustomAttrIds.push_back(id);
}

void VAO::bind() {
	glBindVertexArray(GLVaoId);
	for (GLuint attr = 0; attr < this->GLAttributesIds.size(); attr++) {
		glEnableVertexAttribArray(GLCustomAttrIds[attr]);
	}
}

void VAO::unbind() {
	glBindVertexArray(GLVaoId);
	for (GLuint attr = 0; attr < this->GLAttributesIds.size(); attr++) {
		glDisableVertexAttribArray(GLCustomAttrIds[attr]);
	}
}

VAO::~VAO() {
	glBindVertexArray(this->GLVaoId);
	for (GLuint attrId : GLAttributesIds) {
		glDeleteBuffers(1, &attrId);
	}
	glDeleteVertexArrays(1, &this->GLVaoId);
}
