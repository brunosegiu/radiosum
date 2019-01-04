#pragma once

#include "Renderer.h"

class IDRenderer : public Renderer {
public:
	IDRenderer(Scene* scene);
	void render();
	virtual ~IDRenderer();
};

