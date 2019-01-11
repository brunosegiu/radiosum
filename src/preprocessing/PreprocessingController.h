#pragma once

#include <vector>

#include "preprocessing/IDRenderer.h"
#include "common/geometry/Scene.h"
#include "common/buffers/RowBuffer.h"
#include "shaders/ComputeShader.h"

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

