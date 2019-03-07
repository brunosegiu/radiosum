#pragma once

#include "preprocessing/PreprocessingController.h"
#include "display/DisplayController.h"
#include "display/SceneRenderer.h"
#include "common/geometry/Scene.h"
#include "display/Window.h"
#include "common/AppTypes.h"
#include "common/buffers/PickingBuffer.h"

class Application {
public:
	static void init();
	static void run();

	static void setMode(RunMode mode);
	static void computeRadiosity();
	static void startPreprocessPass();

	static RunMode getMode();
	static float getProgress();
	static Scene* getScene();
	static Window* getWindow();
	static void attachListener(EventListener* listener);
	static std::vector<EventListener*> getEventListeners();

	static PickingBuffer* getPickingBuffer();

private:
	Scene* currentScene;

	Window *window;
	SDL_Window* sdlWin;
	SDL_GLContext glContext;

	PreprocessingController* preprocessor;
	SceneRenderer* sceneRenderer;
	DisplayController* displayer;
	UI* ui;

	RunMode mode;
	GLuint step;

	void update();

	Application();
	virtual ~Application();
	static Application* instance;
	static Application* getInstance();

	static std::vector<EventListener*> listeners;
};

