#pragma once

#include <GL/glew.h>

class Texture {
public:
	Texture(GLuint width, GLuint height, GLenum target = GL_TEXTURE_2D, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE);
	void bind();
	GLuint read(GLenum texture = GL_TEXTURE0);
	virtual ~Texture();
private:
	GLuint GLId;
	GLenum target;
};

