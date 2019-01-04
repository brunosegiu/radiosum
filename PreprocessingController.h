#pragma once

#include <vector>

#include "IDRenderer.h"
#include "Scene.h"

class PreprocessingController {
public:
	PreprocessingController(Scene* scene);
	void reset();
	void runStep();
	bool end();
	virtual ~PreprocessingController();
private:
	IDRenderer* renderer;
	std::vector<Mesh*> meshes;
	unsigned int meshStep, faceStep;
	Camera* cam;
};

