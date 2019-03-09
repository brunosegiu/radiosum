#pragma once

#include "EngineTypes.h"
#include "display/TextureRenderer.h"
#include "display/SceneRenderer.h"
#include "Camera.h"

class DisplayController {
public:
	DisplayController(RenderMode mode);
	void render();
	GLuint read();
	void setMode(RenderMode mode);
	virtual ~DisplayController();
private:

	Renderer* renderer;
	TextureRenderer* bufferRenderer;
	TextureRenderer* radiosityRenderer;
	TextureRenderer* emissionsRenderer;
	TextureRenderer* facesRenderer;
	TextureRenderer* reflactancesRenderer;

	Camera* userCamera;

	RenderMode mode;
};

