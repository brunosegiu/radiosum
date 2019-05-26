#pragma once
#include <vector>

#include <GL/glew.h>

#include "buffers/Buffer.h"

class IDBuffer : public Buffer {
 public:
  IDBuffer(GLuint width);
  GLuint read();
  std::vector<GLuint> getData();
  void cleanStencil();
  virtual ~IDBuffer();

 private:
  GLuint GLId, GLTextureId, GLDepthId, GLStencilId;
};
