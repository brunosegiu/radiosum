#pragma once

#include <string>
#include <vector>

#include <GL/GLU.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "EventListener.h"

class Window : public EventListener {
 public:
  Window();

  bool open();
  void process(SDL_Event& event);

  SDL_Window* getSDLWindow();
  SDL_GLContext getGlContext();

  virtual ~Window();

 private:
  GLuint width, height;
  std::string name;
  SDL_Window* window;
  SDL_GLContext glContext;
  bool isOpen;
};
