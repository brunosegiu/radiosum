#pragma once

#include "Renderer.h"

class ReflectionsRenderer : public Renderer {
 public:
  ReflectionsRenderer(Scene *scene);
  void render();
  void render(Face &face, GLuint faceIndex, Face &reflectiveFace,
              GLuint reflectiveFaceIndex);
  std::vector<GLuint> getData();
  virtual ~ReflectionsRenderer();

 private:
  glm::vec4 clipPlane;
  Shader *stencilShader;
};
