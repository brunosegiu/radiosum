#pragma once
#include "Renderer.h"
class SceneRenderer : public Renderer {
public:
	SceneRenderer(Scene* scene);
	void render();
	virtual ~SceneRenderer();
};

