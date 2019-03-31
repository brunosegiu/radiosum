#include "buffers/Buffer.h"

Buffer::Buffer(GLuint width, GLuint height) {
  this->width = width;
  this->height = height;
}

void Buffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);
  glViewport(0, 0, this->width, this->height);
}

void Buffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Buffer::clean() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

GLuint Buffer::read() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, GLTextureId);
  return GLTextureId;
}

Buffer::~Buffer() {}