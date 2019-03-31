#pragma once

#include <ctime>

#include <GL/glew.h>

#include "EventListener.h"

class CameraController : public EventListener {
 public:
  CameraController(SDL_Window* win);

  void update();
  void toggleIn();
  void process(SDL_Event& event);

  virtual ~CameraController();

 private:
  float sensitivity, speed;
  SDL_Window* win;
  int mouseX, mouseY;
  std::clock_t lastUpdate;
  bool in, set;
  GLuint width, height;
};
