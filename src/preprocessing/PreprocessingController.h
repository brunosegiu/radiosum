#pragma once

#include <vector>

#include <Sparse> 

#include "preprocessing/IDRenderer.h"
#include "common/geometry/Scene.h"
#include "common/buffers/RowBuffer.h"
#include "shaders/ComputeShader.h"

class PreprocessingController {
public:
	PreprocessingController(Scene* scene);
	std::vector<float> computeRadiosity();
	void runStep();
	virtual ~PreprocessingController();
private:
	Scene* scene;

	IDRenderer* renderer;
	Camera* cam;

	ComputeShader* reducer;
	RowBuffer* row;
	GLuint instances;

	SceneIterator* iterator;

	Eigen::SparseMatrix<GLfloat> matrix;
	GLfloat pixelCount;

	std::vector<GLuint> getMatrixRow(GLuint face);
};

