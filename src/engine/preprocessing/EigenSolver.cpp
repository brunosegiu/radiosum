#include "preprocessing/EigenSolver.h"

EigenSolver::EigenSolver() {}

void EigenSolver::init(
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

void EigenSolver::multiplyReflactance(std::vector<GLfloat> reflactance) {
  for (int k = 0; k < matrix.outerSize(); ++k) {
    for (Eigen::SparseMatrix<GLfloat>::InnerIterator it(matrix, k); it; ++it) {
      it.valueRef() = it.row() == it.col()
                          ? it.value()
                          : -it.value() * reflactance[it.row()];
    }
  }
}

std::vector<GLfloat> EigenSolver::solve(std::vector<GLfloat> emissions) {
  // Initialize solver
  Eigen::SparseLU<Eigen::SparseMatrix<GLfloat>> solver;
  solver.analyzePattern(matrix);
  solver.factorize(matrix);

  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }

  // Get emissions
  Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> emissionsXf(emissions.data(),
                                                            emissions.size());
  Eigen::VectorXf _radiosity(emissionsXf.size());

  // Compute radiosity
  _radiosity = solver.solve(emissionsXf);
  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }

  std::vector<GLfloat> radiosity =
      std::vector<GLfloat>(size_t(_radiosity.size()), 0.0f);
  Eigen::VectorXf::Map(&radiosity[0], _radiosity.size()) = _radiosity;
  return radiosity;
}

EigenSolver::~EigenSolver() {}
