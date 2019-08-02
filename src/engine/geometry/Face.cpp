#include "geometry/Face.h"

Face::Face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
  this->v0 = v0;
  this->v1 = v1;
  this->v2 = v2;
  this->mode = TRIANGLE;
}

Face::Face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
  this->v0 = v0;
  this->v1 = v1;
  this->v2 = v2;
  this->v3 = v3;
  this->mode = QUAD;
}

glm::vec3 Face::getNormal() {
  glm::vec3 u = v1 - v0;
  glm::vec3 v = v2 - v0;
  return glm::normalize(glm::cross(u, v));
}

glm::vec3 Face::getUp() { return glm::normalize(v0 - v1); }

glm::vec3 Face::getBarycenter() {
  if (mode == TRIANGLE)
    return (v0 + v1 + v2) / 3.0f;
  else
    return (v0 + v1 + v2 + v3) / 4.0f;
}

glm::vec4 Face::getPlane() {
  glm::vec3 normal = this->getNormal();
  float d = glm::dot(normal, -v0);
  return glm::vec4(normal, d);
}

GLfloat Face::area() {
  if (mode == TRIANGLE) {
    glm::vec3 u = glm::cross(v1 - v0, v2 - v1);
    return .5f * glm::length(u);
  } else {
    glm::vec3 u1 = glm::cross(v1 - v0, v2 - v1);
    glm::vec3 u2 = glm::cross(v2 - v1, v3 - v2);
    return .5f * (glm::length(u1) + glm::length(u2));
  }
}

std::vector<glm::vec3> Face::getSamplingPoints() {
  if (mode == TRIANGLE) {
    std::vector<glm::vec3> points = {
        v0,
        v1,
        v2,
        (v0 + v1) / 2.0f,
        (v1 + v2) / 2.0f,
        (v0 + v2) / 2.0f,
        this->getBarycenter(),
    };
    return points;
  } else {
    std::vector<glm::vec3> points = {v0,
                                     v1,
                                     v2,
                                     v3,
                                     (v0 + v1) / 2.0f,
                                     (v0 + v2) / 2.0f,
                                     (v0 + v3) / 2.0f,
                                     (v1 + v2) / 2.0f,
                                     (v1 + v3) / 2.0f,
                                     (v2 + v3) / 2.0f,
                                     this->getBarycenter()};
    return points;
  }
}

Face::~Face() {}