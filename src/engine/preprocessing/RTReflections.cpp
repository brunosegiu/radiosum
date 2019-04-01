#include "preprocessing/RTReflections.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <map>

#include <gtx/rotate_vector.hpp>

#define MAX_SAMPLES 10000
#define ADDITION 1.0f / MAX_SAMPLES

RTReflections::RTReflections(Scene* scene) : ReflectionsPipeline(scene) {
  this->nscene = scene;
  this->reflactances = this->nscene->getEmission();
  this->device = rtcNewDevice("threads=0");
  this->scene = rtcNewScene(this->device);
  this->reflactances = reflactances;
  auto geometry = scene->getGeometry();
  for (auto& mesh : geometry) {
    RTCBuffer vertices =
        rtcNewSharedBuffer(device, mesh.vertices.vertices.data(),
                           sizeof(glm::vec3) * mesh.vertices.vertices.size());
    RTCGeometry triGeom =
        rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_TRIANGLE);
    rtcSetGeometryBuffer(triGeom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                         vertices, 0, 0, mesh.vertices.vertices.size());
    GLuint* indexT = (GLuint*)rtcSetNewGeometryBuffer(
        triGeom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, sizeof(GLuint),
        mesh.triangles.size());
    for (GLuint i = 0; i < mesh.triangles.size(); i++) {
      indexT[i] = mesh.triangles[i];
    }
    rtcCommitGeometry(triGeom);
    rtcAttachGeometry(this->scene, triGeom);

    RTCGeometry quadGeom =
        rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_TRIANGLE);
    rtcSetGeometryBuffer(quadGeom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                         vertices, 0, 0, mesh.vertices.vertices.size());
    GLuint* indexQ = (GLuint*)rtcSetNewGeometryBuffer(
        triGeom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, sizeof(GLuint),
        mesh.quads.size());
    for (GLuint i = 0; i < mesh.quads.size(); i++) {
      indexQ[i] = mesh.quads[i];
    }
    rtcCommitGeometry(quadGeom);
    rtcAttachGeometry(this->scene, quadGeom);
  }

  rtcCommitScene(this->scene);

  // https://stackoverflow.com/questions/9878965/rand-between-0-and-1
  std::mt19937_64 rng;
  uint64_t timeSeed =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
  rng.seed(ss);
  uniformGenerator = std::uniform_real_distribution<GLfloat>(.0f, 1.0f);
}

GLuint RTReflections::renderRay(RTCRay ray) {
  RTCIntersectContext context;
  rtcInitIntersectContext(&context);
  RTCRayHit hit;
  hit.ray = ray;
  rtcIntersect1(this->scene, &context, &hit);
  if (hit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
    GLuint face = hit.hit.geomID;
    if (this->reflactances[face] > .0f) {
      glm::vec3 rayDir = glm::vec3(hit.ray.dir_x, hit.ray.dir_y, hit.ray.dir_z);
      glm::vec3 orig = glm::vec3(hit.ray.org_x, hit.ray.org_y, hit.ray.org_z) +
                       hit.ray.tfar * rayDir;
      glm::vec3 hitNormal =
          glm::normalize(glm::vec3(hit.hit.Ng_x, hit.hit.Ng_y, hit.hit.Ng_z));
      glm::vec3 dir = 2 * glm::dot(hitNormal, -rayDir) * hitNormal + rayDir;
      RTCRay reflected = RTCRay();
      reflected.org_x = orig.x;
      reflected.org_y = orig.y;
      reflected.org_z = orig.z;
      reflected.dir_x = dir.x;
      reflected.dir_y = dir.y;
      reflected.dir_z = dir.z;
      return renderRay(reflected);
    } else {
      return face;
    }
  }
}

void RTReflections::runWr(std::vector<Face> faces) {
#pragma omp parallel for
  for (int i = 0; i < nscene->size(); i++) {
    Face face = faces[i];
    glm::vec3 orig = face.getBarycenter();
    glm::vec3 normal = face.getNormal();
    std::map<GLuint, GLfloat> tempFF;
    // Normal is up, get angle between normal and (0,1,0)

    GLfloat rotationAngle = glm::acos(glm::dot(normal, glm::vec3(0, 1, 0)));

    for (GLuint samples = 0; samples < MAX_SAMPLES; samples++) {
      glm::vec3 hemDir;
      glm::vec2 rand(uniformGenerator(rng), uniformGenerator(rng));

      // http://www.trapzz.com/?page_id=346
      glm::vec2 vect(rand.x * 2.0f * M_PI, sqrt(1.0f - rand.y));

      hemDir.x = cos(vect.x) * vect.y;
      hemDir.y = sqrt(rand.y);
      hemDir.z = sin(vect.x) * vect.y;

      glm::vec3 dir = glm::rotate(hemDir, rotationAngle, normal);

      RTCRay ray = RTCRay();

      ray.org_x = orig.x;
      ray.org_y = orig.y;
      ray.org_z = orig.z;
      ray.dir_x = dir.x;
      ray.dir_y = dir.y;
      ray.dir_z = dir.z;

      GLuint faceId = this->renderRay(ray);
      if (faceId != 0) {
        if (tempFF.count(faceId) > 0) {
          tempFF[faceId] += ADDITION;
        } else {
          tempFF[faceId] = ADDITION;
        }
      }
    }
    for (auto& pair : tempFF) {
#pragma omp critical
      triplets.push_back(
          std::tuple<GLuint, GLuint, GLfloat>(i, pair.first, pair.second));
    }
  }
  done = true;
}

void RTReflections::run() {
  std::vector<Face> faces;
  faces.reserve(nscene->size());
  for (GLuint i = 0; i < nscene->size(); i++) {
    faces.push_back(nscene->getFace(i));
  }
  if (this->main.joinable()) this->main.join();
  this->main = std::thread(&RTReflections::runWr, this, faces);
}

RTReflections::~RTReflections() {
  rtcReleaseScene(this->scene);
  rtcReleaseDevice(this->device);
}
