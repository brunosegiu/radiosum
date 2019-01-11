#pragma once

#include "display/BufferRenderer.h"
#include "display/SceneRenderer.h"

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

