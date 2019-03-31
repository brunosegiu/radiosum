#pragma once

#include "CameraController.h"
#include "Engine.h"
#include "UI.h"
#include "Window.h"

class Application {
 public:
  Application();
  virtual ~Application();
  void setEngine(Engine* engine);
  void update();
  void loop();
  void attachListener(EventListener* listener);

 private:
  Window* window;
  SDL_Window* sdlWin;
  SDL_GLContext glContext;

  Engine* engine;

  UI* ui;

  CameraController* cameraController;

  std::vector<EventListener*> listeners;
};
