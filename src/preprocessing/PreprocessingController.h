#pragma once

#include <vector>
#include <thread>
#include <mutex>

#include <Sparse> 

#include "preprocessing/IDRenderer.h"
#include "common/geometry/Scene.h"
#include "common/buffers/RowBuffer.h"
#include "shaders/ComputeShader.h"
#include "preprocessing/HemicubeCorrector.h"

class PreprocessingController {
public:
	PreprocessingController(Scene* scene);
	void computeRadiosity();
	GLuint runStep();
	virtual ~PreprocessingController();
private:
	Scene* scene;

	IDRenderer* renderer;
	Camera* cam;

	ComputeShader* reducer;
	RowBuffer* row;
	HemicubeCorrector* corrector;
	GLuint instances;

	SceneIterator* iterator;

	std::vector<Eigen::Triplet<GLfloat>> triplets;
	GLfloat pixelCount;

	std::vector<std::thread> workers;
	std::mutex tripletsLock;

	std::vector<GLfloat> getMatrixRow(GLuint face);
	void processRow(std::vector<GLfloat> faceFactors, GLuint faceIndex);
};

