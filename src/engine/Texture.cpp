#include "Texture.h"
#include <SDL.h>
#include <SDL_image.h>

Texture* Texture::load(std::string path) {
  SDL_Surface* sur = IMG_Load(path.data());
  GLenum mode = sur->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
  return new Texture(sur->w, sur->h, GL_TEXTURE_2D, mode, mode,
                     GL_UNSIGNED_BYTE, sur->pixels);
}

Texture::Texture(GLuint width, GLuint height, GLenum target,
                 GLint internalFormat, GLenum format, GLenum type,
                 GLvoid* data) {
  this->GLId = 0;
  this->target = target;
  glGenTextures(1, &this->GLId);
  glBindTexture(target, this->GLId);
  if (target == GL_TEXTURE_2D) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type,
                 data);
  }
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void Texture::bind() { glBindTexture(this->target, this->GLId); }

void Texture::unbind(GLuint texture) {
  glActiveTexture(GL_TEXTURE0 + texture);
  glBindTexture(this->target, 0);
  glDisable(target);
}

GLuint Texture::read(GLuint texture, bool cs) {
  if (cs)
    glBindImageTexture(texture, this->GLId, 0, GL_FALSE, 0, GL_READ_ONLY,
                       GL_R32F);
  else {
    this->bind();
    glActiveTexture(GL_TEXTURE0 + texture);
    glEnable(this->target);
  }
  return this->GLId;
}

Texture::~Texture() {
  this->bind();
  glDeleteTextures(1, &this->GLId);
}
