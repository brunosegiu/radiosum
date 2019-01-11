#pragma once

#include <vector>

#include "IDRenderer.h"
#include "Scene.h"
#include "ComputeShader.h"
#include "RowBuffer.h"

class PreprocessingController {
public:
	PreprocessingController(Scene* scene);
	void runStep();
	virtual ~PreprocessingController();
private:
	IDRenderer* renderer;
	Scene* scene;
	Camera* cam;
	ComputeShader* reducer;
	RowBuffer* row;
	GLuint instances;
	SceneIterator* iterator;

	std::vector<GLuint> getMatrixRow(GLuint face);
};

