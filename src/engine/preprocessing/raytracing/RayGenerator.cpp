#include "preprocessing/raytracing/RayGenerator.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <gtx/rotate_vector.hpp>

RayGenerator::RayGenerator() {
  // https://stackoverflow.com/questions/9878965/rand-between-0-and-1
  uint64_t timeSeed =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
  rng.seed(ss);
  uniformGenerator = std::uniform_real_distribution<GLfloat>(.0f, 1.0f);
}

glm::vec3 RayGenerator::getHemisphereDir(glm::vec3 &origDir) {
  glm::vec3 dir;

  glm::vec2 rand(uniformGenerator(rng), uniformGenerator(rng));

  const GLfloat r = sqrtf(rand.x);
  const GLfloat theta = 2.0f * M_PI * rand.y;

  const GLfloat x = r * cosf(theta);
  const GLfloat y = r * sinf(theta);

  return glm::vec3(x, sqrtf(1.0f - rand.x), y);
}
