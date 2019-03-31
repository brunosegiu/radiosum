#pragma once

#include "Renderer.h"

class TextureRenderer : public Renderer {
 public:
  TextureRenderer(std::string fragName);
  void render();
  GLuint read();
  virtual ~TextureRenderer();

 private:
  Shader* shader;
  GLuint GLQuadId, GLVerticesId;
};