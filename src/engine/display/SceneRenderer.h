#pragma once

#include "Renderer.h"

class SceneRenderer : public Renderer {
public:
	SceneRenderer(Camera* camera);
	void render();
	void read();
	virtual ~SceneRenderer();
};

