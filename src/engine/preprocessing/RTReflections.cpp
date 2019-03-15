#include "preprocessing/RTReflections.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <map>

#include <embree3/rtcore.h>
#include <embree3/rtcore_geometry.h>

#include <gtx/rotate_vector.hpp>

#define MAX_SAMPLES 10000
#define ADDITION 1.0f / MAX_SAMPLES

RTReflections::RTReflections(std::vector<GeometryBuffers> geometry) : ReflectionsPipeline(geometry) {
	this->device = rtcNewDevice("threads=0");
	this->scene = rtcNewScene(this->device);

	for (auto &mesh : geometry) {
		RTCGeometry triGeom = rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_TRIANGLE);
		rtcSetNewGeometryBuffer(handler, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(glm::vec3), mesh.triangles.vertices.size());
		rtcCommitGeometry(triGeom);
		rtcAttachGeometry(this->scene, triGeom);

		RTCGeometry quadGeom = rtcNewGeometry(this->device, RTC_GEOMETRY_TYPE_TRIANGLE);
		rtcSetNewGeometryBuffer();
		rtcCommitGeometry(quadGeom);
		rtcAttachGeometry(this->scene, quadGeom);
	}

	rtcCommitScene(this->scene);

	//https://stackoverflow.com/questions/9878965/rand-between-0-and-1
	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
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
		if (hit.hit.reflection) { // pseudocode
			glm::vec3 orig = hit.hit;
			glm::vec3 dir = 2 * glm::dot(normal, -ray.dir) * normal + ray.dir;
			RTCRay reflected = RTCRay();
			reflected.org_x = orig.x;	reflected.org_y = orig.y;	reflected.org_z = orig.z;
			reflected.dir_x = dir.x;	reflected.dir_y = dir.y;	reflected.dir_z = dir.z;
			return renderRay(reflected);
		}
	}
}

std::vector<std::tuple<GLuint, GLfloat>> RTReflections::compute(Face* face) {
	glm::vec3 orig = face->getBarycenter();
	glm::vec3 normal = face->getNormal();
	std::vector<std::tuple<GLuint, GLfloat>> ff;
	std::map<GLuint, GLfloat> tempFF;
	// Normal is up, get angle between normal and (0,1,0)

	GLfloat rotationAngle = glm::acos(glm::dot(normal, glm::vec3(0, 1, 0)));

	for (GLuint samples = 0; samples < MAX_SAMPLES; samples++) {
		glm::vec3 hemDir;
		glm::vec2 rand(uniformGenerator(rng), uniformGenerator(rng));

		//http://www.trapzz.com/?page_id=346
		glm::vec2 vect(rand.x * 2.0f * M_PI, sqrt(1.0f - rand.y));

		hemDir.x = cos(vect.x) * vect.y;
		hemDir.y = sqrt(rand.y);
		hemDir.z = sin(vect.x) * vect.y;

		glm::vec3 dir = glm::rotate(hemDir, rotationAngle, normal);

		RTCRay ray = RTCRay();

		ray.org_x = orig.x;	ray.org_y = orig.y;	ray.org_z = orig.z;
		ray.dir_x = dir.x;	ray.dir_y = dir.y;	ray.dir_z = dir.z;

		GLuint faceId = this->renderRay(ray);
		if (faceId != 0) {
			if (tempFF.count(faceId) > 0) {
				tempFF[faceId] += ADDITION;
			}
			else {
				tempFF[faceId] = ADDITION;
			}
		}
	}
	for (auto &pair : tempFF) {
		ff.push_back(std::tuple<GLuint, GLfloat>(pair.first, pair.second));
	}
}

RTReflections::~RTReflections() {
	rtcReleaseScene(this->scene);
	rtcReleaseDevice(this->device);
}
