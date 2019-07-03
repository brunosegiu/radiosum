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

Mesh::Mesh(IndexedBuffers geometry, Material *material) {
  this->geometry = geometry;
  this->tFaces = geometry.vertices.triangles.size() / 3;
  this->qFaces = geometry.vertices.quads.size() / 4;
  this->faces = tFaces + qFaces;

  // Compute adjacencies
  adjacencies = std::vector<std::vector<GLuint>>(geometry.vertices.data.size(),
                                                 std::vector<GLuint>());
  for (GLuint i = 0; i < geometry.vertices.triangles.size(); i++) {
    adjacencies[geometry.vertices.triangles[i]].push_back(i / 3);
  }
  for (GLuint i = 0; i < geometry.vertices.quads.size(); i++) {
    adjacencies[geometry.vertices.quads[i]].push_back(tFaces + (i / 4));
  }

  // Flatten faces (1 face -> 3/6 vertices)
  std::vector<glm::vec3> triangles =
      getFlattened<IndexedBuffer<glm::vec3>, glm::vec3>(geometry.vertices);
  std::vector<glm::vec2> textures =
      getFlattened<IndexedBuffer<glm::vec2>, glm::vec2>(geometry.textures);

  /*
     std::vector<glm::vec3> normals =
         getFlattened<IndexedBuffer<glm::vec3>, glm::vec3>(geometry.normals);
     std::vector<glm::vec3> faceNormals =
         std::vector<glm::vec3>(normals.size(), glm::vec3(.0f, 1.0f, .0f));

     if (normals.size()) {
       for (GLuint i = 0; i < triangles.size(); i += 3) {
         Face f = Face(triangles[i], triangles[i + 1], triangles[i + 2]);
         auto normal = f.getNormal();
         faceNormals[i + 0] = normal;
         faceNormals[i + 1] = normal;
         faceNormals[i + 2] = normal;
       }
     }
 */

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
  /*
  if (faceNormals.size())
    this->vao->addAttribute(sizeof(glm::vec3) * faceNormals.size(),
                            &faceNormals[0].x, 3, GL_FLOAT, NORMALS_ID);
  if (normals.size())
    this->vao->addAttribute(sizeof(glm::vec3) * normals.size(), &normals[0].x,
                            3, GL_FLOAT, PHONG_NORMALS_ID);
  */

  this->simpleGeometry = new VBO();
  this->simpleGeometry->addVertices(geometry.vertices.data);
  std::vector<GLuint> indices = geometry.vertices.triangles;
  for (GLuint i = 0; i < geometry.vertices.quads.size(); i += 4) {
    std::vector<GLuint> quad =
        std::vector<GLuint>(geometry.vertices.quads.begin() + i,
                            geometry.vertices.quads.begin() + i + 4);
    std::vector<GLuint> triangulated = triangulate(quad);
    indices.insert(indices.end(), triangulated.begin(), triangulated.end());
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
            this->radiosity, this->adjacencies[this->geometry.vertices
                                                   .triangles[3 * i + vertex]]);
      }
    }
    for (GLuint i = 0; i < qFaces; i++) {
      glm::vec3 value = radiosity[tFaces + i];
      perVertexRadiosity[3 * tFaces + 6 * i + 0] = interpolate(
          this->radiosity,
          this->adjacencies[this->geometry.vertices.quads[4 * i + 0]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 1] = interpolate(
          this->radiosity,
          this->adjacencies[this->geometry.vertices.quads[4 * i + 1]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 2] = interpolate(
          this->radiosity,
          this->adjacencies[this->geometry.vertices.quads[4 * i + 3]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 3] = interpolate(
          this->radiosity,
          this->adjacencies[this->geometry.vertices.quads[4 * i + 1]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 4] = interpolate(
          this->radiosity,
          this->adjacencies[this->geometry.vertices.quads[4 * i + 2]]);
      perVertexRadiosity[3 * tFaces + 6 * i + 5] = interpolate(
          this->radiosity,
          this->adjacencies[this->geometry.vertices.quads[4 * i + 3]]);
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

void Mesh::setSpecularReflactance(GLuint faceIndex,
                                  GLfloat specularReflactance) {
  this->material->setSpecularReflactance(faceIndex, specularReflactance);
}

void Mesh::setSpecularReflactance(GLfloat specularReflactance) {
  this->material->setSpecularReflactance(specularReflactance);
}

void Mesh::setSpecularReflactance(std::vector<GLfloat> specularReflactance) {
  this->material->setSpecularReflactance(specularReflactance);
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
  GLuint texturedLoc = glGetUniformLocation(shaderID, "textured");
  glUniform1ui(texturedLoc, this->material->textured() ? 1 : 0);
  vao->bind();
  glDrawArrays(GL_TRIANGLES, 0, GLsizei(3 * this->tFaces + 6 * qFaces));
  vao->unbind();
  this->material->unbind();
}

void Mesh::drawGeometry(GLuint shaderID) {
  GLuint tFacesLoc = glGetUniformLocation(shaderID, "tFaces");
  glUniform1ui(tFacesLoc, this->tFaces);
  this->simpleGeometry->bind();
  glDrawElements(GL_TRIANGLES, GLsizei(3 * tFaces + 6 * qFaces),
                 GL_UNSIGNED_INT, (void *)(0));
  this->simpleGeometry->unbind();
}

void Mesh::drawFace(GLuint shaderID, GLuint faceIndex) {
  GLuint tFacesLoc = glGetUniformLocation(shaderID, "tFaces");
  glUniform1ui(tFacesLoc, this->tFaces);
  this->simpleGeometry->bind();
  if (faceIndex < this->tFaces) {
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                   (void *)(3 * faceIndex * sizeof(GLuint)));
  } else {
    GLuint inQuad = faceIndex - tFaces;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   (void *)((3 * tFaces + inQuad * 6) * sizeof(GLuint)));
  }
  this->simpleGeometry->unbind();
}

IndexedBuffers Mesh::getGeometry() { return this->geometry; }

std::vector<GLfloat> Mesh::getEmission() {
  return this->material->getEmission();
}

GLfloat Mesh::getEmission(GLuint faceIndex) {
  return this->material->getEmission(faceIndex);
}

std::vector<GLfloat> Mesh::getSpecularReflactance() {
  return this->material->getSpecularReflactance();
}

GLfloat Mesh::getSpecularReflactance(GLuint faceIndex) {
  return this->material->getSpecularReflactance(faceIndex);
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
    indices[0] = this->geometry.vertices.triangles[3 * index];
    indices[1] = this->geometry.vertices.triangles[3 * index + 1];
    indices[2] = this->geometry.vertices.triangles[3 * index + 2];
    return Face(this->geometry.vertices.data[indices[0]],
                this->geometry.vertices.data[indices[1]],
                this->geometry.vertices.data[indices[2]]);
  } else {
    GLuint inQuad = index - tFaces;
    std::vector<GLuint> indices(4);
    indices[0] = this->geometry.vertices.quads[4 * inQuad + 0];
    indices[1] = this->geometry.vertices.quads[4 * inQuad + 1];
    indices[2] = this->geometry.vertices.quads[4 * inQuad + 2];
    indices[3] = this->geometry.vertices.quads[4 * inQuad + 3];
    return Face(this->geometry.vertices.data[indices[0]],
                this->geometry.vertices.data[indices[1]],
                this->geometry.vertices.data[indices[2]],
                this->geometry.vertices.data[indices[3]]);
  }
}

Mesh::~Mesh() {
  delete this->vao;
  delete this->simpleGeometry;
}
