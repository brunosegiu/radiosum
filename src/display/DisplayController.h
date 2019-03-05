#pragma once

#include "display/TextureRenderer.h"
#include "display/SceneRenderer.h"
#include "display/ui/UI.h"
#include "common/AppTypes.h"

class DisplayController {
public:
	DisplayController();
	void render();
	void setMode(RunMode mode);
	virtual ~DisplayController();
private:
	Renderer* renderer;
	TextureRenderer* bufferRenderer;
	TextureRenderer* textureRenderer;
	UI* ui;
	RunMode mode;
};

