#pragma once

#include <embree3/rtcore.h>
#include <glm.hpp>

inline RTCRay getRay(const glm::vec3& orig, const glm::vec3& dir) {
  RTCRay ray;

  ray.org_x = orig.x;
  ray.org_y = orig.y;
  ray.org_z = orig.z;

  ray.dir_x = dir.x;
  ray.dir_y = dir.y;
  ray.dir_z = dir.z;

  ray.tfar = 5000.0f;
  ray.tnear = 0.01f;
  ray.time = 0;

  ray.flags = 0;

  return ray;
}

inline glm::mat3 getBase(glm::vec3 normal) {
  glm::vec3 nt, nb;

  if (std::fabs(normal.x) > std::fabs(normal.y))
    nt = glm::vec3(normal.z, 0, -normal.x) /
         sqrtf(normal.x * normal.x + normal.z * normal.z);
  else
    nt = glm::vec3(.0f, -normal.z, normal.y) /
         sqrtf(normal.y * normal.y + normal.z * normal.z);
  nb = glm::cross(normal, nt);

  return glm::mat3(nb, normal, nt);
}