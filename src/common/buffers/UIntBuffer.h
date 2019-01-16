#pragma once

#include <GL/glew.h>

#include "common/buffers/Buffer.h"

class UIntBuffer : public Buffer {
public:
	UIntBuffer(GLuint width, GLuint height);
	void bind();
	void read();
	virtual ~UIntBuffer();

private:
	GLuint GLId, GLTextureId, GLDepthId;
};