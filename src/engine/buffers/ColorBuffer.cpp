#include "buffers/ColorBuffer.h"

#include <stdexcept>

ColorBuffer::ColorBuffer(GLuint width, GLuint height) : Buffer(width, height) {
  this->GLId = 0;
  this->GLTextureId = 0;

  // Generate buffer
  glGenFramebuffers(1, &this->GLId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);

  // Generate radiosity texture
  glGenTextures(1, &this->GLTextureId);
  glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Attach elements to buffer
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLTextureId,
                       0);

  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Error configuring cubemap buffer");
  }
}

void ColorBuffer::clean() { glClear(GL_COLOR_BUFFER_BIT); }

ColorBuffer::~ColorBuffer() {
  this->bind();
  glDeleteTextures(1, &this->GLTextureId);
}
