#pragma once

#include <imgui.h>

#include "Component.h"

class DebugInfo : public Component {
 public:
  DebugInfo();
  void render();
  virtual ~DebugInfo();

 private:
  int X_OFFSET = 10.0f;
  int Y_OFFSET = 20.0f;
  ImVec2 appRes;
};