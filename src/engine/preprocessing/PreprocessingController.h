#pragma once

#include <vector>
#include <thread>
#include <mutex>

#include <Sparse> 

#include "EngineTypes.h"
#include "preprocessing/IDRenderer.h"
#include "geometry/Scene.h"
#include "buffers/RowBuffer.h"
#include "shaders/ComputeShader.h"
#include "preprocessing/HemicubeCorrector.h"

class PreprocessingController {
public:
	PreprocessingController(Scene* scene);
	GLuint runStep();
	void runUnsafe(bool full = false);
	void computeRadiosity(std::vector<Channel> channels, bool smooth = false);
	void checkFlags();
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

	std::vector<std::thread> formFactorWorkers;
	std::mutex tripletsLock;

	std::vector<std::thread> radiosityWorkers;
	bool shouldUpdateGeom[N_CHANNELS];
	bool shouldInterpolate;
	GLuint channelCount;
	std::vector<GLfloat> radiosity[N_CHANNELS];

	void setUpRenderer();

	void waitForWorkers();
	std::vector<GLfloat> getMatrixRow(GLuint face);
	void processRow(std::vector<GLfloat> faceFactors, GLuint faceIndex);
	void crWrapped(Channel channel);
};

