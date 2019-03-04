#pragma once

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


#include "display/EventListener.h"

class UI : public EventListener {
public:
	UI(SDL_Window* window, SDL_GLContext &glContext);
	void process(SDL_Event& event);
	void render();
	virtual ~UI();
private:
	SDL_Window* window;
};

