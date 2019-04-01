#pragma once

#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>

typedef struct VertexPack {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> textures;
} VertexPack;

typedef struct IndexedBuffers {
  VertexPack vertices;
  std::vector<GLuint> triangles;
  std::vector<GLuint> quads;
} IndexedBuffers;

typedef struct FlattenedBuffers {
  VertexPack triangles;
  VertexPack quads;
} FlattenedBuffers;