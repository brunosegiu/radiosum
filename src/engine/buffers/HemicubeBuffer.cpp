#include "buffers/HemicubeBuffer.h"

#include <stdexcept>

HemicubeBuffer::HemicubeBuffer(GLuint width) : Buffer(width, width) {
  this->GLId = 0;
  this->GLTextureId = 0;
  this->GLDepthId = 0;

  // Generate buffers
  glGenFramebuffers(1, &this->GLId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);

  // Generate ID's textures, single uint value per pixel
  glGenTextures(1, &this->GLTextureId);
  glBindTexture(GL_TEXTURE_2D_ARRAY, this->GLTextureId);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R32UI, width, width, HEMICUBE_FACES,
               0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);

  // Generate depth buffer (so that depth test works)
  glGenTextures(1, &this->GLDepthId);
  glBindTexture(GL_TEXTURE_2D_ARRAY, this->GLDepthId);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, width, width,
               HEMICUBE_FACES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  // Attach elements to buffer
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLTextureId,
                       0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->GLDepthId, 0);

  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Error configuring cubemap buffer");
  }
}

GLuint HemicubeBuffer::read() {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, this->GLTextureId);
  glBindImageTexture(1, this->GLTextureId, 0, GL_TRUE, 0, GL_READ_ONLY,
                     GL_R32UI);
  return GLTextureId;
}

std::vector<std::vector<GLuint>> HemicubeBuffer::getData() {
  GLuint dataSize = this->width * this->width * HEMICUBE_FACES;
  GLuint* data = new GLuint[dataSize];
  std::vector<std::vector<GLuint>> buffers;

  glBindTexture(GL_TEXTURE_2D_ARRAY, this->GLTextureId);
  glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, data);

  for (GLuint face = 0; face < HEMICUBE_FACES; face++) {
    std::vector<GLuint> wrapped =
        std::vector<GLuint>(data + (this->width * this->width) * face,
                            data + (this->width * this->width) * (face + 1));
    buffers.push_back(wrapped);
  }
  delete data;
  return buffers;
}

HemicubeBuffer::~HemicubeBuffer() {
  this->bind();
  glDeleteTextures(1, &this->GLTextureId);
  glDeleteRenderbuffers(HEMICUBE_FACES, &this->GLDepthId);
  glDeleteBuffers(HEMICUBE_FACES, &this->GLId);
}
