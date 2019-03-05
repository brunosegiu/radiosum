#include "common/buffers/PickingBuffer.h"

#include <stdexcept>

PickingBuffer::PickingBuffer(GLuint width, GLuint height) : Buffer(width, height) {
	this->GLId = 0;
	this->GLTextureId = 0;
	this->GLDepthId = 0;
	this->GLPickingTextureId = 0;

	// Generate buffer
	glGenFramebuffers(1, &this->GLId);
	glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);

	// Generate ID's texture, single uint value per pixel
	glGenTextures(1, &this->GLTextureId);
	glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Generate ID's texture, single uint value per pixel
	glGenTextures(1, &this->GLPickingTextureId);
	glBindTexture(GL_TEXTURE_2D, this->GLPickingTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Generate depth buffer (so that depth test works)
	glGenRenderbuffers(1, &this->GLDepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, this->GLDepthId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->GLDepthId);

	// Attach elements to buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLTextureId, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->GLPickingTextureId, 0);
	GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Error configuring cubemap buffer");
	}
}


void PickingBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);
	glViewport(0, 0, this->width, this->width);
}

void PickingBuffer::read() {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
}

void PickingBuffer::clean() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

PickingBuffer::~PickingBuffer() {
	this->bind();
	glDeleteTextures(1, &this->GLTextureId);
	glDeleteTextures(1, &this->GLPickingTextureId);
	glDeleteTextures(1, &this->GLTextureId);
}
