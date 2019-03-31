#include "buffers/PickingBuffer.h"

#include <stdexcept>

PickingBuffer::PickingBuffer(GLuint width, GLuint height)
    : Buffer(width, height) {
  this->GLId = 0;
  this->GLTextureId = 0;
  this->GLPickingTextureId = 0;
  this->GLEmissionsId = 0;
  this->GLReflactanceId = 0;

  // Generate buffer
  glGenFramebuffers(1, &this->GLId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);

  // Generate radiosity texture
  glGenTextures(1, &this->GLTextureId);
  glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT,
               0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  // Generate ID's texture, single uint value per pixel
  glGenTextures(1, &this->GLPickingTextureId);
  glBindTexture(GL_TEXTURE_2D, this->GLPickingTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER,
               GL_UNSIGNED_INT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  // Generate emissions texture
  glGenTextures(1, &this->GLEmissionsId);
  glBindTexture(GL_TEXTURE_2D, this->GLEmissionsId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT,
               0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  // Generate reflactance texture
  glGenTextures(1, &this->GLReflactanceId);
  glBindTexture(GL_TEXTURE_2D, this->GLReflactanceId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT,
               0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  // Generate depth buffer (so that depth test works)
  glGenRenderbuffers(1, &this->GLDepthId);
  glBindRenderbuffer(GL_RENDERBUFFER, this->GLDepthId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, this->GLDepthId);

  // Attach elements to buffer
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLTextureId,
                       0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                       this->GLPickingTextureId, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
                       this->GLEmissionsId, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
                       this->GLReflactanceId, 0);

  GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                       GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
  glDrawBuffers(4, buffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Error configuring picking buffer");
  }
}

GLuint PickingBuffer::read() {
  this->bind();
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, this->GLPickingTextureId);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, this->GLEmissionsId);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, this->GLReflactanceId);
  return GLTextureId;
}

GLuint PickingBuffer::getSelectedItem(GLuint x, GLuint y) {
  this->bind();
  GLuint data;
  glReadBuffer(GL_COLOR_ATTACHMENT1);
  if (x <= width && y <= height && height - y - 1 >= 0)
    glReadPixels(x, height - y - 1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT,
                 &data);
  else
    data = 0;
  this->unbind();
  return data;
}

void PickingBuffer::clean() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

PickingBuffer::~PickingBuffer() {
  this->bind();
  glDeleteTextures(1, &this->GLTextureId);
  glDeleteTextures(1, &this->GLPickingTextureId);
  glDeleteTextures(1, &this->GLTextureId);
  glDeleteTextures(1, &this->GLReflactanceId);
}
