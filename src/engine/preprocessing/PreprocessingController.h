#pragma once

#include <vector>
#include <thread>
#include <mutex>

#include <Sparse> 

#include "preprocessing/IDRenderer.h"
#include "geometry/Scene.h"
#include "buffers/RowBuffer.h"
#include "shaders/ComputeShader.h"
#include "preprocessing/HemicubeCorrector.h"

class PreprocessingController {
public:
	PreprocessingController(Scene* scene);
	void computeRadiosity(bool smooth = false);
	void checkFlags();
	GLuint runStep();
	void runUnsafe(bool full = false);
	virtual ~PreprocessingController();
private:
	Scene* scene;

	IDRenderer* renderer;
	Camera* cam;

	ComputeShader* reducer;
	RowBuffer* row;
	HemicubeCorrector* corrector;
	GLuint instances;
	bool shouldUpdateGeom, shouldInterpolate, singleChannel;

	SceneIterator* iterator;

	std::vector<Eigen::Triplet<GLfloat>> triplets;
	GLfloat pixelCount;

	std::vector<std::thread> workers;
	std::thread radiosityThread;
	std::mutex tripletsLock;
	std::vector<GLfloat> vectorizedRad;

	void setUpRenderer();

	void waitForWorkers();
	std::vector<GLfloat> getMatrixRow(GLuint face);
	void processRow(std::vector<GLfloat> faceFactors, GLuint faceIndex);
	void crWrapped();
};

