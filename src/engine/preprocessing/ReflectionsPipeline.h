#pragma once

#include <vector>

#include <glm.hpp>

#include "geometry/Face.h"
#include "geometry/GeometryContainer.h"
#include "geometry/Scene.h"

class ReflectionsPipeline {
 public:
  inline ReflectionsPipeline(Scene* scene){};
  virtual void run() = 0;
  inline virtual ~ReflectionsPipeline(){};
};
