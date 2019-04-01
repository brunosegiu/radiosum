#pragma once

#include <string>

#include <GL/glew.h>

class Texture {
 public:
  Texture(GLuint width, GLuint height, GLenum target = GL_TEXTURE_2D,
          GLint internalFormat = GL_RGB, GLenum format = GL_RGB,
          GLenum type = GL_UNSIGNED_BYTE, GLvoid* data = nullptr);
  void bind();
  void unbind(GLuint texture);
  GLuint read(GLenum texture = 0, bool cs = false);
  static Texture* load(std::string path);
  virtual ~Texture();

 private:
  GLuint GLId;
  GLenum target;
};
