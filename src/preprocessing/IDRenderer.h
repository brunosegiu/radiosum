#pragma once

#include "common/Renderer.h"

class IDRenderer : public Renderer {
public:
	IDRenderer(Scene* scene);
	void render();
	void setClipPlane(glm::vec4 plane);
	virtual ~IDRenderer();
private:
	glm::vec4 clipPlane;
};

