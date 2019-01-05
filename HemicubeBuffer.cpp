#include "HemicubeBuffer.h"


HemicubeBuffer::HemicubeBuffer(GLuint width, GLuint height) : Buffer(width, height) {
	this->GLId = 0;
	this->GLTextureId = 0;
	this->GLDepthId = 0;

	// Generate buffers
	glGenFramebuffers(1, &this->GLId);
	glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);

	// Generate ID's textures, single uint value per pixel
	glGenTextures(1, &this->GLTextureId);
	glBindTexture(GL_TEXTURE_3D, this->GLTextureId);
	for (GLuint face = GL_TEXTURE_CUBE_MAP_POSITIVE_X; face < 6; ++face) {
		glTexImage2D(face, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	// Generate depth buffer (so that depth test works)
	glGenRenderbuffers(1, &this->GLDepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, this->GLDepthId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->GLDepthId);

	// Attach elements to buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLTextureId, 0);
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, buffers);
}

void HemicubeBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);
}

void HemicubeBuffer::read() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, this->GLTextureId);
}

HemicubeBuffer::~HemicubeBuffer() {
	this->bind();
	glDeleteTextures(1, &this->GLTextureId);
	glDeleteRenderbuffers(6, &this->GLDepthId);
	glDeleteBuffers(6, &this->GLId);
}
