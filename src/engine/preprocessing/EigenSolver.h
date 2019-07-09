#pragma once

#include <vector>

#include <GL/glew.h>
#include <IterativeLinearSolvers>
#include <Sparse>
#include <SparseCholesky>
#include <SparseLU>
#include <SparseQR>

#include "preprocessing/Solver.h"

typedef Eigen::SparseLU<Eigen::SparseMatrix<GLfloat>> SparseLU;
typedef Eigen::SimplicialLDLT<Eigen::SparseMatrix<GLfloat>> SimplicialLDLT;
typedef Eigen::ConjugateGradient<Eigen::SparseMatrix<GLfloat>>
    ConjugateGradient;
typedef Eigen::BiCGSTAB<Eigen::SparseMatrix<GLfloat>> BiCGSTAB;

class EigenSolver : public Solver {
 public:
  void init(GLuint size,
            std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets);
  void multiplyReflactance(std::vector<GLfloat> reflactance);
  virtual void initSolver() = 0;
  virtual Eigen::VectorXf solve(
      Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector) = 0;
  std::vector<GLfloat> solve(std::vector<GLfloat> emissions);

 protected:
  Eigen::SparseMatrix<GLfloat> matrix;
};

class EigenSolverSparseLU : public EigenSolver {
 public:
  void initSolver();
  Eigen::VectorXf solve(Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector);

 private:
  SparseLU solver;
};

class EigenSolverSimplicialLDLT : public EigenSolver {
 public:
  void initSolver();
  Eigen::VectorXf solve(Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector);

 private:
  SimplicialLDLT solver;
};

class EigenSolverConjugateGradient : public EigenSolver {
 public:
  void initSolver();
  Eigen::VectorXf solve(Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector);

 private:
  ConjugateGradient solver;
};

class EigenSolverBiCGSTAB : public EigenSolver {
 public:
  void initSolver();
  Eigen::VectorXf solve(Eigen::Map<Eigen::VectorXf, Eigen::Unaligned> &vector);

 private:
  BiCGSTAB solver;
};
