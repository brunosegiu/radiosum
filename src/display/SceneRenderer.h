#pragma once

#include "common/Renderer.h"

class SceneRenderer : public Renderer {
public:
	SceneRenderer();
	void render();
	void read();
	virtual ~SceneRenderer();
};

