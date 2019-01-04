#pragma once

#include <vector>

#include "Scene.h"
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
	Scene* scene;
	Shader* shader;
	Camera* camera;
	Buffer* buffer;
};

