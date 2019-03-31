#pragma once

#include <imgui.h>

#include "Component.h"

class LeftPanel : public Component {
 public:
  LeftPanel();
  void render();
  virtual ~LeftPanel();
};