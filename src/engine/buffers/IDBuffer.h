#pragma once
#include <vector>

#include <GL/glew.h>

#include "buffers/Buffer.h"

class IDBuffer : public Buffer {
 public:
  IDBuffer(GLuint width);
  std::vector<GLuint> getData();
  void clean();
  virtual ~IDBuffer();

 private:
  GLuint GLDepthId;
};
