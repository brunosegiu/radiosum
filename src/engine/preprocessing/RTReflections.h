#pragma once

#include <random>
#include <chrono>

#include "preprocessing/ReflectionsPipeline.h"
#include <embree3/rtcore_ray.h>


class RTReflections : public ReflectionsPipeline {
public:
	RTReflections(std::vector<GeometryBuffers> geometry);
	std::vector<std::tuple<GLuint, GLfloat>> compute(Face* face);
	~RTReflections();
private:
	GLuint renderRay(RTCRay ray);
	RTCDevice device;
	RTCScene scene;
	std::mt19937_64 rng;
	std::uniform_real_distribution<GLfloat> uniformGenerator;
};

