#pragma once

#include <vector>

#include "EngineTypes.h"
#include "geometry/Scene.h"
#include "preprocessing/Pipeline.h"

class PreprocessingController {
public:
	PreprocessingController(Scene* scene);
	GLuint runStep();
	void runUnsafe(bool full = false);
	void computeRadiosity(std::set<Channel> channels, bool smooth = false);
	void checkFlags();
	virtual ~PreprocessingController();
private:
	SceneIterator* iterator;
	Pipeline* pipeline;
};

