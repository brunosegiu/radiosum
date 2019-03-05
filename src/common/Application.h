#pragma once

#include "preprocessing/PreprocessingController.h"
#include "display/DisplayController.h"
#include "display/SceneRenderer.h"
#include "common/geometry/Scene.h"
#include "display/Window.h"
#include "common/AppTypes.h"

class Application {
public:
	static void run();
	static void setMode(RunMode mode);
	static Scene* getScene();
	static void startPreprocessPass();
private:
	Scene* currentScene;

	Window *win;

	PreprocessingController* preprocessor;
	SceneRenderer* sceneRenderer;
	DisplayController* displayer;

	RunMode mode;

	Application();
	virtual ~Application();
	static Application* instance;
	static Application* getInstance();
};

