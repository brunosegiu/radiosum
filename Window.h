#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "EventListener.h"
#include "CameraController.h"

class Window {
public:
	static Window* get();

	void update();

	Camera* getCamera();

	virtual ~Window();
private:
	static  Window* instance;

	Window();

	GLuint width, height;
	std::string name;
	SDL_Window* window;
	SDL_GLContext glContext;
	CameraController* camera;
	std::vector<EventListener*> listeners;
};
