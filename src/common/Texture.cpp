#include "common/Texture.h"


Texture::Texture(GLuint width, GLuint height, GLenum target, GLint internalFormat, GLenum format, GLenum type) {
	this->GLId = 0;
	this->target = target;
	glGenTextures(1, &this->GLId);
	glBindTexture(target, this->GLId);
	if (target == GL_TEXTURE_2D) {
		glTexImage2D(target, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
	}
}

void Texture::bind() {
	glBindTexture(this->target, this->GLId);
}

GLuint Texture::read(GLenum texture) {
	this->bind();
	glActiveTexture(texture);
}


Texture::~Texture() {
	this->bind();
	glDeleteTextures(1, &this->GLId);
}
