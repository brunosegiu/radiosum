#include "geometry/Material.h"

Material::Material(glm::vec3 reflactance, GLfloat emission, Texture* texture) {
  this->reflactance = reflactance;
  this->emission = emission;
  this->texture = texture;
  this->faceCount = this->vertexCount = this->tFaces = 0;
}

void Material::instance(GLuint faceCount, GLuint vertexCount, GLuint tFaces,
                        VAO* vao) {
  this->faceCount = faceCount;
  this->tFaces = tFaces;
  this->vertexCount = vertexCount;
  this->vao = vao;
  this->faceCount = faceCount;
  this->vertexCount = vertexCount;
  this->vao = vao;

  this->emissions.reserve(faceCount);
  this->reflactances.reserve(faceCount);

  for (GLuint i = 0; i < faceCount; i++) {
    this->emissions.push_back(.0f);
    this->reflactances.push_back(glm::vec3(1.0f));
  }

  std::vector<GLfloat> perVertexEmission;
  std::vector<glm::vec3> perVertexReflactance;

  perVertexEmission.reserve(vertexCount);
  perVertexReflactance.reserve(vertexCount);

  for (GLuint i = 0; i < vertexCount; i++) {
    perVertexEmission.push_back(emission);
    perVertexReflactance.push_back(reflactance);
  }
  this->vao->addAttribute(sizeof(GLfloat) * perVertexEmission.size(),
                          &perVertexEmission[0], 1, GL_FLOAT, EMISSION_ID,
                          GL_DYNAMIC_DRAW);
  this->vao->addAttribute(sizeof(glm::vec3) * perVertexReflactance.size(),
                          &perVertexReflactance[0], 3, GL_FLOAT, REFLACTANCE_ID,
                          GL_DYNAMIC_DRAW);
}

void Material::bind() {
  if (texture) {
    this->texture->bind();
    this->texture->read(0);
  }
}

void Material::setEmission(GLuint faceIndex, GLfloat emission) {
  this->emissions[faceIndex] = emission;
  std::vector<GLfloat> perVertex;
  if (faceIndex < tFaces) {
    perVertex = std::vector<GLfloat>(3, emission);
  } else {
    perVertex = std::vector<GLfloat>(6, emission);
  }
  this->vao->updateAttribute(
      faceIndex < tFaces ? faceIndex * 3 : 6 * faceIndex - tFaces * 3,
      sizeof(GLfloat) * perVertex.size(), &perVertex[0], EMISSION_ID);
}

void Material::setEmission(GLfloat emission) {
  std::vector<GLfloat> perVertex;
  perVertex.reserve(vertexCount);
  this->emissions = std::vector<GLfloat>(this->emissions.size(), emission);
  perVertex = std::vector<GLfloat>(vertexCount, emission);
  this->vao->updateAttribute(0, sizeof(GLfloat) * perVertex.size(),
                             &perVertex[0], EMISSION_ID);
}

void Material::setReflactance(GLuint faceIndex, glm::vec3 reflactance) {
  this->reflactances[faceIndex] = reflactance;
  std::vector<glm::vec3> perVertex;
  if (faceIndex < tFaces) {
    perVertex = std::vector<glm::vec3>(3, reflactance);
  } else {
    perVertex = std::vector<glm::vec3>(6, reflactance);
  }
  this->vao->updateAttribute(
      faceIndex < tFaces ? faceIndex * 3 : 6 * faceIndex - tFaces * 3,
      sizeof(glm::vec3) * perVertex.size(), &perVertex[0].x, REFLACTANCE_ID);
}

void Material::setReflactance(glm::vec3 reflactance) {
  std::vector<glm::vec3> perVertex;
  this->reflactances =
      std::vector<glm::vec3>(this->reflactances.size(), reflactance);
  perVertex = std::vector<glm::vec3>(vertexCount, reflactance);
  this->vao->updateAttribute(0, sizeof(glm::vec3) * perVertex.size(),
                             &perVertex[0].x, REFLACTANCE_ID);
}

void Material::setReflactance(std::vector<glm::vec3> reflactance) {
  std::vector<glm::vec3> perVertex;
  this->reflactances = reflactance;
  perVertex = std::vector<glm::vec3>(vertexCount, glm::vec3(.0f));
  for (GLuint i = 0; i < vertexCount; i++) {
    perVertex[i] = reflactance[i / 3];
  }
  this->vao->updateAttribute(0, sizeof(glm::vec3) * perVertex.size(),
                             &perVertex[0].x, REFLACTANCE_ID);
}

void Material::setEmission(std::vector<GLfloat> emission) {
  std::vector<GLfloat> perVertex;
  this->emissions = emission;
  perVertex = std::vector<GLfloat>(vertexCount, .0f);
  for (GLuint i = 0; i < vertexCount; i++) {
    perVertex[i] = emission[i / 3];
  }
  this->vao->updateAttribute(0, sizeof(glm::vec3) * perVertex.size(),
                             &perVertex[0], EMISSION_ID);
}

std::vector<GLfloat> Material::getEmission() { return this->emissions; }

GLfloat Material::getEmission(GLuint faceIndex) {
  return this->emissions[faceIndex];
}

std::vector<glm::vec3> Material::getReflactance() { return this->reflactances; }

glm::vec3 Material::getReflactance(GLuint faceIndex) {
  return this->reflactances[faceIndex];
}

Material::~Material() {
  if (texture) delete texture;
}
