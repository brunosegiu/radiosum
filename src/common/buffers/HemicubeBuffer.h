#pragma once

#include <GL/glew.h>

#include "common/buffers/Buffer.h"

class HemicubeBuffer : public Buffer {
public:
	HemicubeBuffer(GLuint width);
	GLuint read();
	virtual ~HemicubeBuffer();
private:
	GLuint GLDepthId;
};

