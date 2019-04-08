#pragma once

#include <GL/glew.h>

#include "Component.h"

class RenderWindow : public Component {
 public:
  RenderWindow();
  void setTexture(GLuint texture);
  void render();
  void update();
  virtual ~RenderWindow();

 private:
  GLuint texture;
  ImVec2 mousePos;
};
