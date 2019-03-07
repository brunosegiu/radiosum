#pragma once

#include <vector>

#include "common/buffers/Buffer.h"

class RowBuffer : public Buffer {
public:
	RowBuffer(GLuint length);
	void bind();
	void clean();
	std::vector<GLfloat> getBuffer();
	virtual ~RowBuffer();
private:
	GLuint GLId;
	std::vector<GLfloat> content;
};

