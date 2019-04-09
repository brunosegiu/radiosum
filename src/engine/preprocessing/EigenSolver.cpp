#include "preprocessing/EigenSolver.h"

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

std::vector<GLfloat> EigenSolver::solve(std::vector<GLfloat> emissions) {
  // Initialize solver
  this->initSolver();

  // Get emissions
  Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> emissionsXf(emissions.data(),
                                                            emissions.size());
  Eigen::VectorXf _radiosity(emissionsXf.size());

  // Compute radiosity
  _radiosity = this->solve(emissionsXf);

  std::vector<GLfloat> radiosity =
      std::vector<GLfloat>(size_t(_radiosity.size()), 0.0f);
  Eigen::VectorXf::Map(&radiosity[0], _radiosity.size()) = _radiosity;
  return radiosity;
}

void EigenSolver::multiplyReflactance(std::vector<GLfloat> reflactance) {
  for (int k = 0; k < matrix.outerSize(); ++k) {
    for (Eigen::SparseMatrix<GLfloat>::InnerIterator it(matrix, k); it; ++it) {
      it.valueRef() = -it.value() * reflactance[it.row()];
    }
  }

  for (GLuint i = 0; i < reflactance.size(); i++) {
    matrix.coeffRef(i, i) = 1.0f;
  }
}

void EigenSolverSparseLU::initSolver() {
  solver.analyzePattern(matrix);
  solver.factorize(matrix);

  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }
}

Eigen::VectorXf EigenSolverSparseLU::solve(
    Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector) {
  auto res = solver.solve(vector);
  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }
  return res;
}

void EigenSolverSimplicialLDLT::initSolver() {
  solver.analyzePattern(matrix);
  solver.factorize(matrix);

  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }
}

Eigen::VectorXf EigenSolverSimplicialLDLT::solve(
    Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector) {
  solver.solve(vector);
  auto res = solver.solve(vector);
  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }
  return res;
}

void EigenSolverConjugateGradient::initSolver() {
  solver.compute(matrix);

  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }
}

Eigen::VectorXf EigenSolverConjugateGradient::solve(
    Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector) {
  auto res = solver.solve(vector);
  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }
  return res;
}

void EigenSolverBiCGSTAB::initSolver() {
  solver.compute(matrix);

  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }
}

Eigen::VectorXf EigenSolverBiCGSTAB::solve(
    Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector) {
  auto res = solver.solve(vector);
  if (solver.info() != Eigen::Success) {
    throw std::runtime_error(
        "Cannot compute radiosity from form factor matrix");
  }
  return res;
}
