#pragma once

#include <vector>

#include <GL/glew.h>
#include <Sparse>

#include "preprocessing/Solver.h"

class CustomSolver : public Solver {
 public:
  void init(GLuint size,
            std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets);
  void multiplyReflactance(std::vector<GLfloat> reflactance);
  void initSolver();
  std::vector<GLfloat> solve(std::vector<GLfloat> emissions);

 private:
  Eigen::SparseMatrix<GLfloat> matrix;
};
