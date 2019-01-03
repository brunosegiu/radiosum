#pragma once

#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

class Renderer {
public:
	Renderer();
	Renderer(std::vector<Mesh*> meshes, Shader* shader, Camera* camera);
	virtual void render() = 0;
	virtual ~Renderer();
protected:
	std::vector<Mesh*> meshes;
	Shader* shader;
	Camera* camera;
};
