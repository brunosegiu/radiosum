#pragma once

#include <vector>

#include "Texture.h"

class HemicubeCorrector {
 public:
  HemicubeCorrector(GLuint width);
  void read();
  virtual ~HemicubeCorrector();
  std::vector<GLfloat> topData, sideData;

 private:
  Texture *top, *side;
};
