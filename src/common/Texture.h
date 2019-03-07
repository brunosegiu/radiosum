#pragma once

#include <GL/glew.h>

class Texture {
public:
	Texture(GLuint width, GLuint height, GLenum target = GL_TEXTURE_2D, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE, GLvoid* data = nullptr);
	void bind();
	GLuint read(GLenum texture = 0, bool cs = false);
	virtual ~Texture();
private:
	GLuint GLId;
	GLenum target;
};

