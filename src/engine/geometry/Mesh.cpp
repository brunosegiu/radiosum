#include "geometry/Mesh.h"

#include <fstream>

#include "geometry/GeometryUtils.h"

std::vector<Mesh *> Mesh::load(std::string path) {
  std::string fileExt = getFileExtension(path);
  if (fileExt == "obj") {
    std::vector<Mesh *> meshes = loadOBJ(path);
    return meshes;
  }

  throw std::runtime_error("Unable to load mesh, format not supported");
}
template <typename vect>
std::vector<vect> triangulate(std::vector<vect> &quad) {
  vect v0 = quad[0];
  vect v1 = quad[1];
  vect v2 = quad[2];
  vect v3 = quad[3];

  std::vector<vect> triangles;
  triangles.reserve(6);

  triangles.push_back(v0);
  triangles.push_back(v1);
  triangles.push_back(v3);

  triangles.push_back(v1);
  triangles.push_back(v2);
  triangles.push_back(v3);

  return triangles;
}

Mesh::Mesh(IndexedBuffers geometry, Material *material) {
  this->geometry = geometry;
  this->tFaces = geometry.triangles.size() / 3;
  this->qFaces = geometry.quads.size() / 4;
  this->faces = tFaces + qFaces;

  // Compute adjacencies
  adjacencies = std::vector<std::vector<GLuint>>(
      geometry.vertices.vertices.size(), std::vector<GLuint>());
  for (GLuint i = 0; i < geometry.triangles.size(); i++) {
    adjacencies[geometry.triangles[i]].push_back(i / 3);
  }
  for (GLuint i = 0; i < geometry.quads.size(); i++) {
    adjacencies[geometry.quads[i]].push_back(tFaces + (i / 4));
  }

  // Triangulate quads
  FlattenedBuffers flattened = deIndex(geometry);
  std::vector<glm::vec3> triangles = flattened.triangles.vertices;
  for (GLuint i = 0; i < flattened.quads.vertices.size(); i += 4) {
    std::vector<glm::vec3> quad =
        std::vector<glm::vec3>(flattened.quads.vertices.begin() + i,
                               flattened.quads.vertices.begin() + i + 4);
    std::vector<glm::vec3> triangulated = triangulate(quad);
    triangles.insert(triangles.end(), triangulated.begin(), triangulated.end());
  }
  std::vector<glm::vec2> textures = flattened.triangles.textures;
  for (GLuint i = 0; i < flattened.quads.textures.size(); i += 4) {
    std::vector<glm::vec2> quad =
        std::vector<glm::vec2>(flattened.quads.textures.begin() + i,
                               flattened.quads.textures.begin() + i + 4);
    std::vector<glm::vec2> triangulated = triangulate(quad);
    textures.insert(textures.end(), triangulated.begin(), triangulated.end());
  }

  this->vao = new VAO();

  GLuint vertexCount = this->tFaces * 3 + this->qFaces * 6;
  this->radiosity.reserve(this->faces);
  std::vector<glm::vec3> perVertexRadiosity(vertexCount, glm::vec3(.0f));
  for (GLuint i = 0; i < this->faces; i++) {
    this->radiosity.push_back(glm::vec3(.0f));
  }

  this->material = material;
  this->material->instance(this->faces, vertexCount, tFaces, vao);

  this->vao->addAttribute(sizeof(glm::vec3) * triangles.size(), &triangles[0].x,
                          3, GL_FLOAT, VERTICES_ID);
  this->vao->addAttribute(sizeof(glm::vec3) * perVertexRadiosity.size(),
                          &perVertexRadiosity[0].x, 3, GL_FLOAT, RADIOSITY_ID,
                          GL_DYNAMIC_DRAW);
  if (textures.size())
    this->vao->addAttribute(sizeof(glm::vec2) * textures.size(), &textures[0].x,
                            2, GL_FLOAT, TEXTURES_ID);

  this->simpleGeometry = new VBO();
  this->simpleGeometry->addVertices(geometry.vertices.vertices);
  std::vector<GLuint> indices = geometry.triangles;
  for (GLuint i = 0; i < geometry.quads.size(); i += 4) {
    indices.push_back(geometry.quads[i + 0]);
    indices.push_back(geometry.quads[i + 1]);
    indices.push_back(geometry.quads[i + 3]);
    indices.push_back(geometry.quads[i + 1]);
    indices.push_back(geometry.quads[i + 2]);
    indices.push_back(geometry.quads[i + 3]);
  }
  this->simpleGeometry->addIndices(indices);
}

glm::vec3 interpolate(std::vector<glm::vec3> &radiosities,
                      std::vector<GLuint> &adjacencies) {
  glm::vec3 radiosity = glm::vec3(.0f);
  for (GLuint face : adjacencies) {
    radiosity += radiosities[face];
  }
  return radiosity / GLfloat(adjacencies.size());
}

void Mesh::setRadiosity(std::vector<glm::vec3> radiosity, bool smooth) {
  this->radiosity = radiosity;
  std::vector<glm::vec3> perVertexRadiosity(6 * qFaces + tFaces * 3);
  if (smooth) {
    for (GLuint i = 0; i < tFaces; i++) {
      glm::vec3 value = radiosity[i];
      for (GLuint vertex = 0; vertex < 3; vertex++) {
        perVertexRadiosity[3 * i + vertex] = interpolate(
            this->radiosity,
            this->adjacencies[this->geometry.triangles[3 * i + vertex]]);
      }
    }
    for (GLuint i = 0; i < qFaces; i++) {
      glm::vec3 value = radiosity[tFaces + i];
      perVertexRadiosity[3 * tFaces + 6 * i + 0] = interpolate(
          this->radiosity, this->adjacencies[this->geometry.quads[4 * i + 0]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 1] = interpolate(
          this->radiosity, this->adjacencies[this->geometry.quads[4 * i + 1]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 2] = interpolate(
          this->radiosity, this->adjacencies[this->geometry.quads[4 * i + 3]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 3] = interpolate(
          this->radiosity, this->adjacencies[this->geometry.quads[4 * i + 1]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 4] = interpolate(
          this->radiosity, this->adjacencies[this->geometry.quads[4 * i + 2]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 5] = interpolate(
          this->radiosity, this->adjacencies[this->geometry.quads[4 * i + 3]]);
    }
  } else {
    for (GLuint i = 0; i < tFaces; i++) {
      glm::vec3 value = radiosity[i];
      for (GLuint vertex = 0; vertex < 3; vertex++) {
        perVertexRadiosity[3 * i + vertex] = value;
      }
    }
    for (GLuint i = 0; i < qFaces; i++) {
      glm::vec3 value = radiosity[tFaces + i];
      for (GLuint vertex = 0; vertex < 6; vertex++) {
        perVertexRadiosity[3 * tFaces + 6 * i + vertex] = value;
      }
    }
  }
  this->vao->updateAttribute(0, sizeof(glm::vec3) * perVertexRadiosity.size(),
                             &perVertexRadiosity[0].x, RADIOSITY_ID);
}

std::pair<GLuint, GLuint> findMesh(
    std::map<std::string, std::pair<GLuint, GLuint>> &names, GLuint faceIndex) {
  for (auto &limits : names) {
    if (faceIndex >= limits.second.first && faceIndex < limits.second.second)
      return limits.second;
  }
}

void Mesh::setEmission(GLuint faceIndex, GLfloat emission) {
  this->material->setEmission(faceIndex, emission);
}

void Mesh::setEmission(GLfloat emission) {
  this->material->setEmission(emission);
}

void Mesh::setEmission(std::vector<GLfloat> emission) {
  this->material->setEmission(emission);
}

void Mesh::setReflactance(GLuint faceIndex, glm::vec3 reflactance) {
  this->material->setReflactance(faceIndex, reflactance);
}

void Mesh::setReflactance(glm::vec3 reflactance) {
  this->material->setReflactance(reflactance);
}

void Mesh::setReflactance(std::vector<glm::vec3> reflactance) {
  this->material->setReflactance(reflactance);
}

void Mesh::draw(GLuint shaderID) {
  this->material->bind();
  GLuint tFacesLoc = glGetUniformLocation(shaderID, "tFaces");
  glUniform1ui(tFacesLoc, this->tFaces);
  vao->bind();
  glDrawArrays(GL_TRIANGLES, 0, GLsizei(3 * this->tFaces + 6 * qFaces));
  vao->unbind();
}

void Mesh::drawGeometry(GLuint shaderID) {
  GLuint tFacesLoc = glGetUniformLocation(shaderID, "tFaces");
  glUniform1ui(tFacesLoc, this->tFaces);
  this->simpleGeometry->bind();
  glDrawElements(GL_TRIANGLES, GLsizei(3 * this->tFaces + 6 * qFaces),
                 GL_UNSIGNED_INT, (void *)(0));
  this->simpleGeometry->unbind();
}

IndexedBuffers Mesh::getGeometry() { return this->geometry; }

std::vector<GLfloat> Mesh::getEmission() {
  return this->material->getEmission();
}

GLfloat Mesh::getEmission(GLuint faceIndex) {
  return this->material->getEmission(faceIndex);
}

std::vector<glm::vec3> Mesh::getReflactance() {
  return this->material->getReflactance();
}

glm::vec3 Mesh::getReflactance(GLuint faceIndex) {
  return this->material->getReflactance(faceIndex);
}

std::vector<glm::vec3> Mesh::getRadiosity() { return this->radiosity; }

GLuint Mesh::size() { return faces; }

Face Mesh::getFace(GLuint index) {
  index = index % this->size();
  if (index < tFaces) {
    std::vector<GLuint> indices(3);
    indices[0] = this->geometry.triangles[3 * index];
    indices[1] = this->geometry.triangles[3 * index + 1];
    indices[2] = this->geometry.triangles[3 * index + 2];
    return Face(this->geometry.vertices.vertices[indices[0]],
                this->geometry.vertices.vertices[indices[1]],
                this->geometry.vertices.vertices[indices[2]]);
  } else {
    GLuint inQuad = index - tFaces;
    std::vector<GLuint> indices(4);
    indices[0] = this->geometry.quads[4 * inQuad + 0];
    indices[1] = this->geometry.quads[4 * inQuad + 1];
    indices[2] = this->geometry.quads[4 * inQuad + 2];
    indices[3] = this->geometry.quads[4 * inQuad + 3];
    return Face(this->geometry.vertices.vertices[indices[0]],
                this->geometry.vertices.vertices[indices[1]],
                this->geometry.vertices.vertices[indices[2]],
                this->geometry.vertices.vertices[indices[3]]);
  }
}

Mesh::~Mesh() {
  delete this->vao;
  delete this->simpleGeometry;
}
