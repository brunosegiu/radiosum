#include "geometry/Scene.h"

#include "EngineStore.h"

Scene::Scene() { this->_size = 0; }

void Scene::draw(GLuint shaderID, bool justGeometry) {
  GLuint offset = 1;
  for (auto &mesh : meshes) {
    GLuint offestLoc = glGetUniformLocation(shaderID, "offset");
    glUniform1ui(offestLoc, offset);
    if (justGeometry)
      mesh->drawGeometry(shaderID);
    else
      mesh->draw(shaderID);
    offset += mesh->size();
  }
}

GLuint Scene::size() { return this->_size; }

Face Scene::getFace(GLuint index) {
  GLuint meshIndex = 0;
  Mesh *mesh = this->getMeshWithIndex(index, meshIndex);
  return mesh->getFace(meshIndex);
}

std::vector<GLfloat> Scene::getEmission() {
  std::vector<GLfloat> emissions;
  for (auto mesh : meshes) {
    std::vector<GLfloat> meshEmissions = mesh->getEmission();
    emissions.insert(emissions.end(), meshEmissions.begin(),
                     meshEmissions.end());
  }
  return emissions;
}

std::vector<IndexedBuffers> Scene::getGeometry() {
  std::vector<IndexedBuffers> geom;
  for (auto &mesh : meshes) {
    geom.push_back(mesh->getGeometry());
  }
  return geom;
}

std::vector<glm::vec3> Scene::getReflactance() {
  std::vector<glm::vec3> reflactances;
  for (auto mesh : meshes) {
    std::vector<glm::vec3> meshReflactances = mesh->getReflactance();
    reflactances.insert(reflactances.end(), meshReflactances.begin(),
                        meshReflactances.end());
  }
  return reflactances;
}

std::vector<glm::vec3> Scene::getRadiosity() {
  std::vector<glm::vec3> radiosities;
  for (auto mesh : meshes) {
    std::vector<glm::vec3> meshRadiosity = mesh->getRadiosity();
    radiosities.insert(radiosities.end(), meshRadiosity.begin(),
                       meshRadiosity.end());
  }
  return radiosities;
}

GLfloat Scene::getEmission(GLuint faceIndex) {
  GLuint meshIndex = 0;
  if (_size > 0) {
    Mesh *mesh = this->getMeshWithIndex(faceIndex, meshIndex);
    return mesh->getEmission(meshIndex);
  } else {
    return .0f;
  }
}

glm::vec3 Scene::getReflactance(GLuint faceIndex) {
  GLuint meshIndex = 0;
  if (_size > 0) {
    Mesh *mesh = this->getMeshWithIndex(faceIndex, meshIndex);
    return mesh->getReflactance(meshIndex);
  } else {
    return glm::vec3(.0f);
  }
}

void Scene::addMesh(std::string path) {
  EngineStore::logger.log("Adding mesh from path: " + path);
  std::vector<Mesh *> toAdd = Mesh::load(path);
  for (auto &mesh : toAdd) this->addMesh(mesh);
}

void Scene::addMesh(Mesh *mesh) {
  _size += mesh->size();
  this->meshes.push_back(mesh);
}

void Scene::setRadiosity(std::vector<glm::vec3> &radiosity, bool smooth) {
  GLuint currentIndex = 0;
  for (auto &mesh : this->meshes) {
    mesh->setRadiosity(
        std::vector<glm::vec3>(radiosity.begin() + currentIndex,
                               radiosity.begin() + currentIndex + mesh->size()),
        smooth);
    currentIndex += mesh->size();
  }
}

void Scene::setEmission(GLuint faceIndex, GLfloat emission, bool full) {
  if (_size > 0) {
    GLuint meshIndex = 0;
    Mesh *mesh = this->getMeshWithIndex(faceIndex, meshIndex);
    full ? mesh->setEmission(emission) : mesh->setEmission(meshIndex, emission);
  }
}

void Scene::setReflactance(GLuint faceIndex, glm::vec3 reflactance, bool full) {
  if (_size > 0) {
    GLuint meshIndex = 0;
    Mesh *mesh = this->getMeshWithIndex(faceIndex, meshIndex);
    full ? mesh->setReflactance(reflactance)
         : mesh->setReflactance(meshIndex, reflactance);
  }
}

void Scene::setEmission(std::vector<GLfloat> emission) {
  GLuint offset = 0;
  for (auto &mesh : meshes) {
    std::vector<GLfloat> meshEm = std::vector<GLfloat>(
        emission.begin() + offset, emission.begin() + offset + mesh->size());
    offset += mesh->size();
    mesh->setEmission(meshEm);
  }
}

void Scene::setReflactance(std::vector<glm::vec3> reflactance) {
  GLuint offset = 0;
  for (auto &mesh : meshes) {
    std::vector<glm::vec3> meshRef =
        std::vector<glm::vec3>(reflactance.begin() + offset,
                               reflactance.begin() + offset + mesh->size());
    offset += mesh->size();
    mesh->setReflactance(meshRef);
  }
}

void Scene::setRadiosity(std::vector<glm::vec3> radiosity) {
  GLuint offset = 0;
  for (auto &mesh : meshes) {
    std::vector<glm::vec3> meshRad = std::vector<glm::vec3>(
        radiosity.begin() + offset, radiosity.begin() + offset + mesh->size());
    offset += mesh->size();
    mesh->setRadiosity(meshRad);
  }
}

Mesh *Scene::getMeshWithIndex(GLuint faceIndex, GLuint &meshIndex) {
  GLuint iterator = faceIndex;
  meshIndex = faceIndex;
  for (auto &mesh : this->meshes) {
    if (iterator >= mesh->size()) {
      iterator -= mesh->size();
      meshIndex = iterator;
    } else {
      return mesh;
    }
  }
  throw std::runtime_error("Face index out of range");
}

Scene::~Scene() {
  for (auto mesh : meshes) {
    delete mesh;
  }
}

SceneIterator::SceneIterator(Scene *scene) {
  this->scene = scene;
  this->index = 0;
  this->top = this->scene->size();
}

GLuint SceneIterator::begin() {
  this->index = 0;
  return this->index;
}

GLuint SceneIterator::nextFace() {
  if (!this->end()) this->index += 1;
  return this->index;
}

GLuint SceneIterator::nextVertex() {
  if (!this->end()) this->index++;
  return this->index;
}

Face SceneIterator::get() { return this->scene->getFace(index); }

GLuint SceneIterator::faceIndex() { return this->index; }

bool SceneIterator::end() { return this->index >= this->top; }
