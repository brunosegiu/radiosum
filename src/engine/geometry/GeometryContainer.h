#pragma once

#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

typedef struct IndexedBuffers {
  std::vector<glm::vec3> vertices;
  std::vector<GLuint> triangles;
  std::vector<GLuint> quads;
} IndexedBuffers;

typedef struct FlattenedBuffers {
  std::vector<glm::vec3> triangles;
  std::vector<glm::vec3> quads;
} FlattenedBuffers;