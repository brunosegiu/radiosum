#pragma once

#include "vector"

#include <GL/glew.h>

#include "buffers/Buffer.h"

#define HEMICUBE_FACES 5

class HemicubeBuffer : public Buffer {
 public:
  HemicubeBuffer(GLuint width);
  std::vector<std::vector<GLuint>> getData();
  GLuint read();
  virtual ~HemicubeBuffer();

 private:
  GLuint GLDepthId;
};
