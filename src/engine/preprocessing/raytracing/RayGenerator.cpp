#include "preprocessing/raytracing/RayGenerator.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <gtx/rotate_vector.hpp>

RayGenerator::RayGenerator(GLuint &nRays) {
  GLfloat tilespering = 1;
  GLfloat rings = ceil((-1 + sqrt(nRays)) / 2);
  nRays = 1 + 4 * rings * (rings + 1);
  GLfloat dRho = 1 / (2 * rings + 1);
  this->directions.reserve(nRays);
  for (GLfloat ring = 1; ring <= 2 * rings + 1; ring += 2) {
    GLfloat dTheta = 2 * M_PI / tilespering;
    GLfloat rho = (ring - 1.0) * dRho;
    for (GLfloat k = 1; k <= tilespering; k++) {
      GLfloat theta = (k - 0.5) * dTheta;
      GLfloat x = rho * cos(theta);
      GLfloat y = rho * sin(theta);
      GLfloat z = sqrt(1.0 - x * x - y * y);

      this->directions.push_back(glm::vec3(x, z, y));
    }
    tilespering = (ring + 1) * 4;
  }
}

glm::vec3 RayGenerator::getHemisphereDir(GLuint step) {
  return this->directions[step];
}
