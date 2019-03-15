#pragma once

#include "Logger.h"
#include "EngineTypes.h"

class EngineStore {
public:
	static Logger logger;
	static GLfloat ffProgress;
	static GLfloat radiosityProgress;
	static PipelineStage pipelineStage;
};