#pragma once

#include <GL/glew.h>

#include "Buffer.h"

class HemicubeBuffer : public Buffer {
public:
	HemicubeBuffer(GLuint width);
	void bind();
	void read();
	virtual ~HemicubeBuffer();
private:
	GLuint GLId, GLTextureId, GLDepthId;
};

