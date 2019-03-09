#pragma once

#include <mutex>
#include <condition_variable>

#include "EngineTypes.h"

#include "Camera.h"
#include "geometry/Scene.h"

#include "Renderer.h"

#include "display/SceneRenderer.h"
#include "preprocessing/PreprocessingController.h"

#include "display/DisplayController.h"


class Engine {

public:
	Engine();

	// Main camera
	Camera* getCamera();

	// Engine output
	void setMode(RenderMode mode);
	RenderMode getMode();
	void preprocess(bool withOutput = true); // if withOutput == false run preprocessor until it finishes, without rendering steps
	void computeRadiosity();

	// Main scene
	Scene* getScene();
	void addMesh(std::string path);
	GLuint pick(GLint x, GLint y);

	void startFrame();
	void loop();

	virtual ~Engine();
public:
	GLuint outputTexture;
private:
	Camera* camera;
	Scene* scene;

	bool preprocessorOutput;

	RenderMode mode;
	SceneRenderer* sceneRenderer;
	PreprocessingController* preprocessor;
	DisplayController* displayer;

	std::mutex mtx;
	std::unique_lock<std::mutex> lock;
	std::condition_variable frameLock;
};

