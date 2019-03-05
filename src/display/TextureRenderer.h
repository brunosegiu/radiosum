#pragma once

#include "common/Renderer.h"

class TextureRenderer : public Renderer {
public:
	TextureRenderer(std::string fragName);
	void render();
	virtual ~TextureRenderer();
private:
	Shader* shader;
	GLuint GLQuadId, GLVerticesId;
};