#include "common/buffers/Buffer.h"

Buffer::Buffer(GLuint width, GLuint height) {
	this->width = width;
	this->height = height;
}

void Buffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Buffer::clean() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Buffer::read() {

}

Buffer::~Buffer() {

}