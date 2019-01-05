#pragma once

#include <GL/glew.h>

#include "Buffer.h"

class HemicubeBuffer : public Buffer {
public:
	HemicubeBuffer(GLuint width, GLuint height);
	void bind();
	void read();
	virtual ~HemicubeBuffer();
private:
	GLuint GLId, GLTextureId, GLDepthId;
};

