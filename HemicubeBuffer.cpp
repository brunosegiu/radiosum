#include "HemicubeBuffer.h"


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
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->GLTextureId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32UI, width, width, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
	}

	// Generate depth buffer (so that depth test works)
	glGenTextures(1, &this->GLDepthId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->GLDepthId);
	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}

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
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->GLTextureId);
}

HemicubeBuffer::~HemicubeBuffer() {
	this->bind();
	glDeleteTextures(1, &this->GLTextureId);
	glDeleteRenderbuffers(6, &this->GLDepthId);
	glDeleteBuffers(6, &this->GLId);
}
