#pragma once

#include "common/Renderer.h"

class SceneRenderer : public Renderer {
public:
	SceneRenderer(Scene* scene);
	void render();
	virtual ~SceneRenderer();
};

