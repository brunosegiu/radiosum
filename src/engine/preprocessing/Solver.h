#pragma once

#include <GL/glew.h>

class Solver {
 public:
  inline Solver(){};
  virtual void init(
      GLuint size,
      std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets) = 0;
  virtual void multiplyReflactance(std::vector<GLfloat> reflactance) = 0;
  virtual std::vector<GLfloat> solve(std::vector<GLfloat> emissions) = 0;
  inline ~Solver(){};
};