#pragma once

#include "BufferRenderer.h"
#include "SceneRenderer.h"

class DisplayController {
public:
	DisplayController(Scene* scene);
	void render();
	virtual ~DisplayController();
private:
	Renderer* renderer;
	BufferRenderer* bufferRenderer;
	SceneRenderer* sceneRenderer;
};

