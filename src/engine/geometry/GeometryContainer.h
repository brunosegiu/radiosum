#pragma once

#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

template <typename vectType>
struct IndexedBuffer {
  std::vector<vectType> data;
  std::vector<GLuint> triangles;
  std::vector<GLuint> quads;
};

typedef struct IndexedBuffers {
  IndexedBuffer<glm::vec3> vertices;
  IndexedBuffer<glm::vec2> textures;
  IndexedBuffer<glm::vec3> normals;
} IndexedBuffers;