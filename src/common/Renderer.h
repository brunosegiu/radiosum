#pragma once

#include <vector>

#include "geometry/Scene.h"
#include "shaders/Shader.h"
#include "common/Camera.h"
#include "buffers/Buffer.h"

class Renderer {
public:
	Renderer();
	virtual void render() = 0;
	virtual void setCamera(Camera* camera);
	void read();
	virtual ~Renderer();
protected:
	Scene* scene;
	Shader* shader;
	Camera* camera;
	Buffer* buffer;
};

