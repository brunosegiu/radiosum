#pragma once


#include "common/buffers/Buffer.h"

class PickingBuffer : public Buffer {
public:
	PickingBuffer(GLuint width = 0, GLuint height = 0);
	void bind();
	void read();
	void clean();
	virtual ~PickingBuffer();
private:
	GLuint GLId, GLTextureId, GLDepthId, GLPickingTextureId;
};

