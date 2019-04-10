#pragma once

#include "EngineTypes.h"
#include "Logger.h"

class EngineStore {
 public:
  static Logger logger;
  static GLfloat ffProgress;
  static GLfloat radiosityProgress;
  static GLfloat radiosityScale;
  static PipelineStage pipelineStage;
};