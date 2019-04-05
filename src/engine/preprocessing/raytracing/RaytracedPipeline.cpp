#include "preprocessing/raytracing/RaytracedPipeline.h"

#include "EngineStore.h"

RTCRay getRay(const glm::vec3& orig, const glm::vec3& dir) {
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

RaytracedPipeline::RaytracedPipeline(Scene* scene, GLuint resolution)
    : Pipeline(scene, resolution) {
  this->nRays = resolution * resolution * 3;
  this->reflactances = this->scene->getEmission();
  this->device = rtcNewDevice("threads=0");
  this->eScene = rtcNewScene(this->device);
  this->reflactances = reflactances;
  auto geometry = scene->getGeometry();
  for (auto& mesh : geometry) {
    RTCBuffer vertices =
        rtcNewSharedBuffer(device, mesh.vertices.data.data(),
                           sizeof(glm::vec3) * mesh.vertices.data.size());
    RTCGeometry triGeom =
        rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_TRIANGLE);
    rtcSetGeometryBuffer(triGeom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                         vertices, 0, sizeof(glm::vec3),
                         mesh.vertices.data.size() / 3);
    GLuint* indexT = (GLuint*)rtcSetNewGeometryBuffer(
        triGeom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(GLuint),
        mesh.vertices.triangles.size() / 3);
    for (GLuint i = 0; i < mesh.vertices.triangles.size(); i++) {
      indexT[i] = mesh.vertices.triangles[i];
    }
    rtcCommitGeometry(triGeom);
    rtcAttachGeometry(this->eScene, triGeom);

    RTCGeometry quadGeom = rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_QUAD);
    rtcSetGeometryBuffer(quadGeom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
                         vertices, 0, sizeof(glm::vec3),
                         mesh.vertices.data.size() / 3);
    GLuint* indexQ = (GLuint*)rtcSetNewGeometryBuffer(
        quadGeom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT4,
        4 * sizeof(GLuint), mesh.vertices.quads.size() / 4);
    for (GLuint i = 0; i < mesh.vertices.quads.size(); i++) {
      indexQ[i] = mesh.vertices.quads[i];
    }
    rtcCommitGeometry(quadGeom);
    rtcAttachGeometry(this->eScene, quadGeom);
  }

  rtcCommitScene(this->eScene);
}

std::pair<GLuint, GLfloat> RaytracedPipeline::renderRay(RTCRay ray) {
  RTCIntersectContext context;
  rtcInitIntersectContext(&context);

  RTCRayHit query;
  query.ray = ray;
  query.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  query.hit.primID = RTC_INVALID_GEOMETRY_ID;

  rtcIntersect1(this->eScene, &context, &query);
  if (query.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
    GLuint face = query.hit.primID;
    return std::pair<GLuint, GLfloat>(face, 1.0f / this->nRays);
    if (this->reflactances[face] > .0f) {
      glm::vec3 rayDir =
          glm::vec3(query.ray.dir_x, query.ray.dir_y, query.ray.dir_z);
      glm::vec3 orig =
          glm::vec3(query.ray.org_x, query.ray.org_y, query.ray.org_z) +
          query.ray.tfar * rayDir;
      glm::vec3 hitNormal = glm::normalize(
          glm::vec3(query.hit.Ng_x, query.hit.Ng_y, query.hit.Ng_z));
      glm::vec3 dir = 2 * glm::dot(hitNormal, -rayDir) * hitNormal + rayDir;
      RTCRay reflected = getRay(orig, dir);
      std::pair<GLuint, GLfloat> ff = renderRay(reflected);
      ff.second = this->reflactances[face] * ff.second;
      return ff;
    }
  }
  return std::pair<GLuint, GLfloat>(0, .0f);
}

void RaytracedPipeline::runWr(std::vector<Face> faces) {
  if (!done) {
    for (int i = 0; i < scene->size(); i++) {
      Face face = faces[i];
      glm::vec3 orig = face.getBarycenter();
      glm::vec3 normal = face.getNormal();
      generator.setBaseChangeMatrix(normal);
      ffLock.lock();
      triplets.push_back(std::tuple<GLuint, GLuint, GLfloat>(i, i, 1.0f));
      ffLock.unlock();
      EngineStore::ffProgress += i / GLfloat(scene->size());
      for (GLuint samples = 0; samples < nRays; samples++) {
        glm::vec3 dir = normal;
        // generator.getDir(normal);
        RTCRay ray = getRay(orig, dir);
        std::pair<GLuint, GLfloat> ff = this->renderRay(ray);
        if (ff.second > .0f) {
          ffLock.lock();
          triplets.push_back(
              std::tuple<GLuint, GLuint, GLfloat>(i, ff.first, ff.second));
          ffLock.unlock();
        }
      }
    }
    done = true;
  }
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
