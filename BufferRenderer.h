#pragma once
#include "Renderer.h"
class BufferRenderer : public Renderer {
public:
	BufferRenderer();
	void render();
	virtual ~BufferRenderer();
private:
	Shader* shader;
	GLuint GLQuadId, GLVerticesId;
};

