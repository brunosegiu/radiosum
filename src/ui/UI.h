#pragma once

#include <map>

#include <GL/glew.h>
#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "Component.h"
#include "EventListener.h"
#include "components/RenderWindow.h"

class UI : public EventListener {
 public:
  UI(SDL_Window* window, SDL_GLContext& glContext);
  void process(SDL_Event& event);
  void setTexture(GLuint texture);
  void render();
  virtual ~UI();

 private:
  SDL_Window* window;
  std::map<std::string, Component*> components;
  RenderWindow* externalComponent;
};
