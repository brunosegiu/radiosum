#include "preprocessing/CustomSolver.h"

#define ITERATION_COUNT 100

void CustomSolver::init(
    GLuint size, std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets) {
  matrix = Eigen::SparseMatrix<GLfloat>(size, size);
  std::vector<Eigen::Triplet<GLfloat>> eigenTriplets;
  eigenTriplets.reserve(triplets.size());
  for (auto &triplet : triplets) {
    GLuint i = std::get<0>(triplet);
    GLuint j = std::get<1>(triplet);
    GLfloat ff = std::get<2>(triplet);
    eigenTriplets.push_back(Eigen::Triplet<GLfloat>(i, j, ff));
  }
  matrix.setFromTriplets(eigenTriplets.begin(), eigenTriplets.end());
}

void CustomSolver::multiplyReflactance(std::vector<GLfloat> reflactance) {
  for (int k = 0; k < matrix.outerSize(); ++k) {
    for (Eigen::SparseMatrix<GLfloat>::InnerIterator it(matrix, k); it; ++it) {
      it.valueRef() = it.value() * reflactance[it.row()];
    }
  }
}

void CustomSolver::initSolver() {}

std::vector<GLfloat> CustomSolver::solve(std::vector<GLfloat> emissions) {
  Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> _emissions(emissions.data(),
                                                           emissions.size());
  Eigen::VectorXf _radiosity = _emissions;
  for (int i = 0; i < ITERATION_COUNT; i++) {
    _radiosity = matrix * _radiosity + _emissions;
  };
  std::vector<GLfloat> radiosity =
      std::vector<GLfloat>(size_t(_radiosity.size()), 0.0f);
  Eigen::VectorXf::Map(&radiosity[0], _radiosity.size()) = _radiosity;
  return radiosity;
}