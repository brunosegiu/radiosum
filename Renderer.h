#pragma once

#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Buffer.h"

class Renderer {
public:
	Renderer();
	virtual void render() = 0;
	virtual void setCamera(Camera* camera);
	void read();
	virtual ~Renderer();
protected:
	std::vector<Mesh*> meshes;
	Shader* shader;
	Camera* camera;
	Buffer* buffer;
};

