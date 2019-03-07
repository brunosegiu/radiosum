#pragma once

#include "common/buffers/Buffer.h" 

class ColorBuffer : public Buffer {
public:
	ColorBuffer(GLuint width = 0, GLuint height = 0);
	void clean();
	virtual ~ColorBuffer();
};