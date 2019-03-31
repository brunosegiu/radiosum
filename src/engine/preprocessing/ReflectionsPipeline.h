#pragma once

#include <vector>

#include <glm.hpp>

#include "geometry/Face.h"
#include "geometry/GeometryContainer.h"

class ReflectionsPipeline {
 public:
  inline ReflectionsPipeline(std::vector<IndexedBuffers> geometry,
                             std::vector<GLfloat> reflactances){};
  virtual std::vector<std::tuple<GLuint, GLfloat>> compute(Face* face) = 0;
  inline virtual ~ReflectionsPipeline(){};
};
