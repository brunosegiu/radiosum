#pragma once

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
	void computeRadiosity(std::vector<Channel> channels, bool smooth = false);
	void setRadiosity(bool smooth);

	// Main scene
	Scene* getScene();
	void resetScene();
	void addMesh(std::string path);
	GLuint pick(GLint x, GLint y);

	void step();

	virtual ~Engine();
public:
	GLuint outputTexture;
private:
	Camera* camera;
	Scene* scene;

	RenderMode mode;
	SceneRenderer* sceneRenderer;
	PreprocessingController* preprocessor;
	DisplayController* displayer;
};

