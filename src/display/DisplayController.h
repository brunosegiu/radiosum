#pragma once

#include "display/TextureRenderer.h"
#include "display/SceneRenderer.h"
#include "display/ui/UI.h"
#include "common/AppTypes.h"

class DisplayController {
public:
	DisplayController(RunMode mode);
	void render();
	GLuint read();
	void setMode(RunMode mode);
	virtual ~DisplayController();
private:
	Renderer* renderer;
	TextureRenderer* bufferRenderer;
	TextureRenderer* radiosityRenderer;
	TextureRenderer* emissionsRenderer;
	TextureRenderer* facesRenderer;
	UI* ui;
	RunMode mode;
};

