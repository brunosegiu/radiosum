#pragma once

#include "display/BufferRenderer.h"
#include "display/SceneRenderer.h"
#include "display/ui/UI.h"

class DisplayController {
public:
	DisplayController(Scene* scene);
	void render();
	virtual ~DisplayController();
private:
	Renderer* renderer;
	BufferRenderer* bufferRenderer;
	SceneRenderer* sceneRenderer;
	UI* ui;
};

