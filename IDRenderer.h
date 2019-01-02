#pragma once

#include "Renderer.h"

class IDRenderer : public Renderer {
public:
	IDRenderer(std::vector<Mesh*> meshes, Camera* camera);
	void render();
	virtual ~IDRenderer();
};

