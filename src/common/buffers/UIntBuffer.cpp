#include "common/buffers/UIntBuffer.h"

#include <stdexcept>

UIntBuffer::UIntBuffer(GLuint width, GLuint height) : Buffer(width, height) {
	this->GLId = 0;
	this->GLTextureId = 0;
	this->GLDepthId = 0;

	// Generate buffer
	glGenFramebuffers(1, &this->GLId);
	glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);

	// Generate ID's texture, single uint value per pixel
	glGenTextures(1, &this->GLTextureId);
	glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Generate depth buffer (so that depth test works)
	glGenRenderbuffers(1, &this->GLDepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, this->GLDepthId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, this->GLDepthId);

	// Attach elements to buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLTextureId, 0);
	GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Error configuring cubemap buffer");
	}
}

void UIntBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);
}

void UIntBuffer::read() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
}

UIntBuffer::~UIntBuffer() {
	this->bind();
	glDeleteTextures(1, &this->GLTextureId);
	glDeleteRenderbuffers(1, &this->GLDepthId);
	glDeleteBuffers(1, &this->GLId);
}
