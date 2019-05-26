#pragma once

#include "CameraController.h"
#include "Engine.h"
#include "Logger.h"

enum ShadingModel { FLAT, GOURAUD };

class UIStore {
 public:
  static Engine* engine;
  static bool selectFace;
  static int selectedFace;
  static bool enablePreprocessRendering;
  static bool enableReflections;
  static ShadingModel shading;
  static CameraController* camera;
};