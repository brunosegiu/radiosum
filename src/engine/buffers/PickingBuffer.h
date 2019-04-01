#pragma once

#include "buffers/Buffer.h"

class PickingBuffer : public Buffer {
 public:
  PickingBuffer(GLuint width = 0, GLuint height = 0);
  GLuint read();
  GLuint getSelectedItem(GLuint x, GLuint y);
  void clean();
  virtual ~PickingBuffer();

 private:
  GLuint GLDepthId, GLPickingTextureId, GLEmissionsId, GLReflactanceId,
      GLAlbedoId;
};
