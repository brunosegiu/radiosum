#pragma once

#include "Engine.h"
#include "Logger.h"

class UIStore {
 public:
  static Engine* engine;
  static bool selectFace;
  static int selectedFace;
  static bool enablePreprocessRendering;
};