#pragma once

#include "BufferRenderer.h"

class DisplayController {
public:
	DisplayController();
	void render();
	virtual ~DisplayController();
private:
	Renderer* renderer;
	BufferRenderer* bufferRenderer;
};

