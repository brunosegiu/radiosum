#include "preprocessing/raytracing/RaytracedPipeline.h"

#include "EngineStore.h"
#include "preprocessing/raytracing/RaytracingUtils.h"

#define MAX_LEVELS 5

RaytracedPipeline::RaytracedPipeline(Scene* scene, GLuint resolution)
    : Pipeline(scene, resolution) {
  this->nRays = resolution * resolution * 3;
  this->device = rtcNewDevice("threads=0");
  this->eScene = rtcNewScene(this->device);
  auto geometry = scene->getGeometry();
  this->reflactances = scene->getSpecularReflactance();
  GLuint currentOffset = 0;
  for (auto& mesh : geometry) {
    RTCBuffer vertices =
        rtcNewSharedBuffer(device, mesh.vertices.data.data(),
                           sizeof(glm::vec3) * mesh.vertices.data.size());
    if (mesh.vertices.triangles.size() > 0) {
      RTCGeometry triGeom =
          rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_TRIANGLE);
      rtcSetGeometryBuffer(triGeom, RTC_BUFFER_TYPE_VERTEX, 0,
                           RTC_FORMAT_FLOAT3, vertices, 0, sizeof(glm::vec3),
                           mesh.vertices.data.size());
      GLuint* indexT = (GLuint*)rtcSetNewGeometryBuffer(
          triGeom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
          3 * sizeof(GLuint), mesh.vertices.triangles.size() / 3);
      for (GLuint i = 0; i < mesh.vertices.triangles.size(); i++) {
        indexT[i] = mesh.vertices.triangles[i];
      }
      rtcCommitGeometry(triGeom);
      rtcAttachGeometry(this->eScene, triGeom);
      offsetMap.push_back(currentOffset);
      currentOffset += mesh.vertices.triangles.size() / 3;
    }
    if (mesh.vertices.quads.size() > 0) {
      RTCGeometry quadGeom =
          rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_QUAD);
      rtcSetGeometryBuffer(quadGeom, RTC_BUFFER_TYPE_VERTEX, 0,
                           RTC_FORMAT_FLOAT3, vertices, 0, sizeof(glm::vec3),
                           mesh.vertices.data.size());
      GLuint* indexQ = (GLuint*)rtcSetNewGeometryBuffer(
          quadGeom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT4,
          4 * sizeof(GLuint), mesh.vertices.quads.size() / 4);
      for (GLuint i = 0; i < mesh.vertices.quads.size(); i++) {
        indexQ[i] = mesh.vertices.quads[i];
      }
      rtcCommitGeometry(quadGeom);
      rtcAttachGeometry(this->eScene, quadGeom);
      offsetMap.push_back(currentOffset);
      currentOffset += mesh.vertices.quads.size() / 4;
    }
  }

  rtcCommitScene(this->eScene);
}

GLuint RaytracedPipeline::renderRayOnce(RTCRay& ray, RTCRayHit& query) {
  RTCIntersectContext context;
  rtcInitIntersectContext(&context);

  query.ray = ray;
  query.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  query.hit.primID = RTC_INVALID_GEOMETRY_ID;

  rtcIntersect1(this->eScene, &context, &query);

  if (query.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
    return offsetMap[query.hit.geomID] + query.hit.primID;
  }
  return RTC_INVALID_GEOMETRY_ID;
}

std::map<GLuint, GLfloat> RaytracedPipeline::renderRay(RTCRay ray) {
  std::map<GLuint, GLfloat> ffs;
  RTCRayHit query;

  GLuint face = renderRayOnce(ray, query);

  GLfloat norm = 1.0f / GLfloat(this->nRays);
  GLfloat ffMultiplier = norm;

  GLuint iterationCount = 0;
  while (face != RTC_INVALID_GEOMETRY_ID && iterationCount <= MAX_LEVELS) {
    iterationCount++;
    GLfloat reflactance = reflactances[face];
    ffs[face] = (1.0f - reflactance) * norm;
    ffMultiplier *= reflactance * norm;
    if (reflactance > .0f) {
      glm::vec3 rayDir =
          glm::vec3(query.ray.dir_x, query.ray.dir_y, query.ray.dir_z);
      glm::vec3 orig =
          glm::vec3(query.ray.org_x, query.ray.org_y, query.ray.org_z) +
          query.ray.tfar * rayDir;
      glm::vec3 hitNormal = glm::normalize(
          glm::vec3(query.hit.Ng_x, query.hit.Ng_y, query.hit.Ng_z));
      glm::vec3 dir = 2 * glm::dot(hitNormal, -rayDir) * hitNormal + rayDir;
      RTCRay reflected = getRay(orig, dir);

      face = renderRayOnce(reflected, query);
    }
  }
  return ffs;
}

void RaytracedPipeline::runWr(std::vector<Face> faces) {
  if (!done) {
#pragma omp parallel for
    for (int i = 0; i < scene->size(); i++) {
      Face face = faces[i];
      glm::vec3 normal = face.getNormal();
      glm::vec3 orig = face.getBarycenter() + normal * 1e-5f;
      glm::mat3 base = getBase(normal);
      EngineStore::ffProgress += i / GLfloat(scene->size());
      std::map<GLuint, GLfloat> row;
      for (GLuint samples = 0; samples < nRays; samples++) {
        glm::vec3 dir = base * generator.getHemisphereDir(normal);
        RTCRay ray = getRay(orig, dir);
        std::map<GLuint, GLfloat> ff = this->renderRay(ray);
        for (auto& pair : ff) {
          row[pair.first] =
              pair.second + (row.count(pair.first) > 0 ? row[pair.first] : .0f);
        }
      }
      ffLock.lock();
      for (auto& pair : row) {
        triplets.push_back(
            std::tuple<GLuint, GLuint, GLfloat>(i, pair.first, pair.second));
      }
      ffLock.unlock();
    }
  }
  done = true;
}

void RaytracedPipeline::computeFormFactors() {
  if (!done) {
    this->setStage(FF_LOADING);
    std::vector<Face> faces;
    faces.reserve(scene->size());
    for (GLuint i = 0; i < scene->size(); i++) {
      faces.push_back(scene->getFace(i));
    }
    if (this->mainThread.joinable()) this->mainThread.join();
    this->mainThread = std::thread(&RaytracedPipeline::runWr, this, faces);
  }
}

bool RaytracedPipeline::ready() { return done; }

void RaytracedPipeline::setStage(PipelineStage stage) {
  this->currentStage = stage;
}
RaytracedPipeline::~RaytracedPipeline() {
  this->mainThread.join();
  rtcReleaseScene(this->eScene);
  rtcReleaseDevice(this->device);
}
