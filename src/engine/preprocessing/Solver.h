#pragma once

#include <vector>

#include <GL/glew.h>

class Solver {
 public:
  virtual void init(
      GLuint size,
      std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets) = 0;
  virtual void multiplyReflactance(std::vector<GLfloat> reflactance) = 0;
  virtual void initSolver() = 0;
  virtual std::vector<GLfloat> solve(std::vector<GLfloat> emissions) = 0;
};
