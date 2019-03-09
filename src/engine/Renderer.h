#pragma once

#include <vector>

#include "geometry/Scene.h"
#include "shaders/Shader.h"
#include "Camera.h"
#include "buffers/Buffer.h"

class Renderer {
public:
	Renderer();
	virtual void render() = 0;

	virtual void setCamera(Camera* camera);
	void setScene(Scene* scene);
	GLuint read();
	Buffer* getBuffer();
	void start();
	Camera* getCamera();

	virtual ~Renderer();
protected:
	Scene* scene;
	Shader* shader;
	Camera* camera;
	Buffer* buffer;
};

