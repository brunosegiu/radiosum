#pragma once

#include <GL/glew.h>

class Buffer {
public:
	Buffer(GLuint width, GLuint height);
	virtual void bind() = 0;
	virtual void read() = 0;
	virtual ~Buffer() = 0;

protected:
	GLuint width, height;
};
