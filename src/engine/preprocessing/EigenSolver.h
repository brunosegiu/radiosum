#pragma once

#include <vector>

#include <GL/glew.h>
#include <Sparse>

#include "preprocessing/Solver.h"

class EigenSolver : public Solver {
 public:
  EigenSolver();
  void init(GLuint size,
            std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets);
  void multiplyReflactance(std::vector<GLfloat> reflactance);
  std::vector<GLfloat> solve(std::vector<GLfloat> emissions);
  ~EigenSolver();

 private:
  Eigen::SparseMatrix<GLfloat> matrix;
};
