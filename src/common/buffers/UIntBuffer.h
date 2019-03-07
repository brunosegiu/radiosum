#pragma once

#include <GL/glew.h>

#include "common/buffers/Buffer.h"

class UIntBuffer : public Buffer {
public:
	UIntBuffer(GLuint width, GLuint height);
	GLuint read();
	virtual ~UIntBuffer();

private:
	GLuint GLId, GLTextureId, GLDepthId;
};
