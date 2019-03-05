#include "RowBuffer.h"

#include <stdexcept>

RowBuffer::RowBuffer(GLuint length) : Buffer(length, 1) {
	this->GLId = 0;
	this->content.insert(this->content.begin(), length, 0);

	// Generate and initialize buffer
	glGenBuffers(1, &this->GLId);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->GLId);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->GLId);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->content.size() * sizeof(GLuint), &this->content[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RowBuffer::bind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->GLId);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->GLId);
}

void RowBuffer::clean() {
	this->bind();
	GLuint* data = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < this->content.size(); i++) {
		data[i] = 0;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

std::vector<GLuint> RowBuffer::getBuffer() {
	this->bind();
	GLuint* data = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	this->content = std::vector<GLuint>(data, data + sizeof(data) / sizeof(GLuint));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	return this->content;
}

RowBuffer::~RowBuffer() {
	this->bind();
	glDeleteBuffers(1, &this->GLId);
}
