#pragma once

#include <vector>

#include <glm.hpp>

#include "geometry/GeometryContainer.h"
#include "geometry/Face.h"

class ReflectionsPipeline {
public:
	inline ReflectionsPipeline(std::vector<GeometryBuffers> geometry) {};
	virtual std::vector<std::tuple<GLuint, GLfloat>> compute(Face* face) = 0;
	inline virtual ~ReflectionsPipeline() {};
};

