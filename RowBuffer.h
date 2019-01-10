#pragma once

#include <vector>

#include "Buffer.h"

class RowBuffer : public Buffer {
public:
	RowBuffer(GLuint length);
	void bind();
	void clean();
	std::vector<GLuint> getBuffer();
	virtual ~RowBuffer();
private:
	GLuint GLId;
	std::vector<GLuint> content;
};

