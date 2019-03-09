#pragma once

#include <GL/glew.h>

class Buffer {
public:
	Buffer(GLuint width = 0, GLuint height = 0);
	virtual void bind();
	virtual void unbind();
	virtual GLuint read();
	virtual void clean();
	virtual ~Buffer();
protected:
	GLuint width, height;
	GLuint GLId, GLTextureId;
};
