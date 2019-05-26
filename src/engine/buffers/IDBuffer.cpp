#include "buffers/IDBuffer.h"

#include <stdexcept>

IDBuffer::IDBuffer(GLuint width) : Buffer(width, width) {
  this->GLId = 0;
  this->GLTextureId = 0;
  this->GLDepthId = 0;

  // Generate buffer
  glGenFramebuffers(1, &this->GLId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->GLId);

  // Generate ID's texture, single uint value per pixel
  glGenTextures(1, &this->GLTextureId);
  glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, width, 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Generate depth buffer (so that depth test works)
  glGenRenderbuffers(1, &this->GLDepthId);
  glBindRenderbuffer(GL_RENDERBUFFER, this->GLDepthId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, width);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, this->GLDepthId);

  // Attach elements to buffer
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->GLTextureId,
                       0);
  GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, buffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Error configuring id buffer");
  }
}

GLuint IDBuffer::read() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->GLTextureId);
  return GLTextureId;
}

std::vector<GLuint> IDBuffer::getData() {
  GLuint dataSize = this->width * this->width;
  GLuint* data = new GLuint[dataSize];

  glBindTexture(GL_TEXTURE_2D_ARRAY, this->GLTextureId);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, data);

  std::vector<GLuint> buffer =
      std::vector<GLuint>(data, data + (this->width * this->width));

  delete data;
  return buffer;
}

void IDBuffer::cleanStencil() { glClear(GL_STENCIL_BUFFER_BIT); }

IDBuffer::~IDBuffer() {
  this->bind();
  glDeleteTextures(1, &this->GLTextureId);
  glDeleteRenderbuffers(1, &this->GLDepthId);
  glDeleteBuffers(1, &this->GLId);
}