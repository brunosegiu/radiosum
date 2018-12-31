#pragma once

#include <string>

#include <GL/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

class Window {
public:
	Window(GLuint width, GLuint height, std::string name);
	void update();
	virtual ~Window();

private:
	GLuint width, height;
	std::string name;
	SDL_Window* window;
	SDL_GLContext glContext;
};
