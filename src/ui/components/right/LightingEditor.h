#pragma once

#include <GL/glew.h>

#include "Component.h"
#include "EventListener.h"

class LightingEditor : public Component {
 public:
  LightingEditor();
  void render();
  virtual ~LightingEditor();

 private:
  GLfloat emission;
  GLfloat reflactance[3];
  GLuint lastSelected;
};
