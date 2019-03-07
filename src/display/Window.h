#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "display/EventListener.h"
#include "display/CameraController.h"

class Window : public EventListener {
public:
	Window();

	bool open();
	void process(SDL_Event &event);

	Camera* getCamera();
	SDL_Window* getSDLWindow();
	SDL_GLContext getGlContext();

	virtual ~Window();
private:
	GLuint width, height;
	std::string name;
	SDL_Window* window;
	SDL_GLContext glContext;
	CameraController* camera;
	bool isOpen;
};
