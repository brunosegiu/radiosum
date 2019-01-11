#include "common/buffers/HemicubeBuffer.h"

#define FACES 5

#include <stdexcept>

HemicubeBuffer::HemicubeBuffer(GLuint width) : Buffer(width, width) {
	this->GLId = 0;
	this->GLTextureId = 0;
	this->GLDepthId = 0;

	// Generate buffers
	glGenFramebuffers(1, &this->GLId);
	glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);

	// Generate ID's textures, single uint value per pixel
	glGenTextures(1, &this->GLTextureId);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->GLTextureId);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R32UI, width, width, FACES, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);

	// Generate depth buffer (so that depth test works)
	glGenTextures(1, &this->GLDepthId);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->GLDepthId);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, width, width, FACES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	// Attach elements to buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLTextureId, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->GLDepthId, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Error configuring cubemap buffer");
	}
}

void HemicubeBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);
}

void HemicubeBuffer::read() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->GLTextureId);
	glBindImageTexture(0, this->GLTextureId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);
}

HemicubeBuffer::~HemicubeBuffer() {
	this->bind();
	glDeleteTextures(1, &this->GLTextureId);
	glDeleteRenderbuffers(FACES, &this->GLDepthId);
	glDeleteBuffers(FACES, &this->GLId);
}
