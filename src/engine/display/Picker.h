#pragma once

#include <GL/glew.h>

#include "buffers/PickingBuffer.h"

class Picker {
 public:
  Picker(GLuint maxSize, PickingBuffer* buffer);
  GLuint getSelected(GLint x, GLint y);
  virtual ~Picker();

 private:
  GLuint maxSize;
  PickingBuffer* buffer;
};
