#pragma once

#include <vector>

#include "IDRenderer.h"
#include "Scene.h"
#include "ComputeShader.h"
#include "RowBuffer.h"

class PreprocessingController {
public:
	PreprocessingController(Scene* scene);
	void reset();
	void runStep();
	bool end();
	virtual ~PreprocessingController();
private:
	IDRenderer* renderer;
	Scene* scene;
	unsigned int faceStep;
	Camera* cam;
	ComputeShader* reducer;
	RowBuffer* row;
	GLuint instances;

	std::vector<GLuint> getMatrixRow(GLuint face);
};

