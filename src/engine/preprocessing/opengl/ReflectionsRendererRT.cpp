#include "preprocessing/opengl/ReflectionsRendererRT.h"

#include <gtc/type_ptr.hpp>

#include "ConfigurationManager.h"
#include "buffers/IDBuffer.h"
#include "preprocessing/raytracing/RaytracingUtils.h"

ReflectionsRendererRT::ReflectionsRendererRT(Scene *scene) {
  auto geometry = scene->getGeometry();
  GLuint currentOffset = 0;
  this->device = rtcNewDevice("threads=0");
  this->eScene = rtcNewScene(this->device);
  for (auto &mesh : geometry) {
    RTCBuffer vertices =
        rtcNewSharedBuffer(device, mesh.vertices.data.data(),
                           sizeof(glm::vec3) * mesh.vertices.data.size());
    if (mesh.vertices.triangles.size() > 0) {
      RTCGeometry triGeom =
          rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_TRIANGLE);
      rtcSetGeometryBuffer(triGeom, RTC_BUFFER_TYPE_VERTEX, 0,
                           RTC_FORMAT_FLOAT3, vertices, 0, sizeof(glm::vec3),
                           mesh.vertices.data.size());
      GLuint *indexT = (GLuint *)rtcSetNewGeometryBuffer(
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
      GLuint *indexQ = (GLuint *)rtcSetNewGeometryBuffer(
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

GLuint ReflectionsRendererRT::renderRayOnce(RTCRay &ray, RTCRayHit &query) {
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

void ReflectionsRendererRT::render(Face &face, GLuint faceIndex,
                                   Face &reflectiveFace,
                                   GLuint reflectiveFaceIndex) {
  glm::vec3 viewPoint = face.getBarycenter();
  std::vector<glm::vec3> samplingPoints = reflectiveFace.getSamplingPoints();
  RTCRayHit query;
  glm::vec3 direction =
      2 * glm::dot(-face.getNormal(), reflectiveFace.getNormal()) -
      face.getNormal();
  for (glm::vec3 point : samplingPoints) {
    GLfloat distance = glm::distance(viewPoint, point);
    glm::vec3 origin = point - direction * distance;
    RTCRay reflected = getRay(origin, direction);
    faceIndex = renderRayOnce(reflected, query);
    if (faceIndex != RTC_INVALID_GEOMETRY_ID) {
      this->buffer.push_back(faceIndex);
    }
  }
}

std::vector<GLuint> ReflectionsRendererRT::getData() {
  std::vector<GLuint> bufferCopy = this->buffer;
  this->buffer.clear();
  return bufferCopy;
}

void ReflectionsRendererRT::render() {
  throw std::exception("Not implemented");
}

ReflectionsRendererRT::~ReflectionsRendererRT() {
  rtcReleaseScene(this->eScene);
  rtcReleaseDevice(this->device);
}
