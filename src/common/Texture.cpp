#include "common/Texture.h"


Texture::Texture(GLuint width, GLuint height, GLenum target, GLint internalFormat, GLenum format, GLenum type, GLvoid* data) {
	this->GLId = 0;
	this->target = target;
	glGenTextures(1, &this->GLId);
	glBindTexture(target, this->GLId);
	if (target == GL_TEXTURE_2D) {
		glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
	}
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void Texture::bind() {
	glBindTexture(this->target, this->GLId);
}

GLuint Texture::read(GLuint texture, bool cs) {
	if (cs)
		glBindImageTexture(texture, this->GLId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	else {
		this->bind();
		glActiveTexture(GL_TEXTURE0 + texture);
	}
	return this->GLId;
}


Texture::~Texture() {
	this->bind();
	glDeleteTextures(1, &this->GLId);
}
