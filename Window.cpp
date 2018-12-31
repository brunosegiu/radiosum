#include "Window.h"

Window::Window(GLuint width = 1280, GLuint height = 800, std::string name = "Display") {
	this->width = width;
	this->height = height;
	this->name = name;
	if (SDL_Init(SDL_INIT_VIDEO) >= 0) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		this->window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL)
			throw std::runtime_error("Failed to initialize SDL");
		glContext = SDL_GL_CreateContext(window);
		if (glContext == NULL)
			throw std::runtime_error("Failed to initialize OpenGL");
		glewExperimental = GL_TRUE;
		glewInit();

		// Load OpenGL setting
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
	else {
		throw std::runtime_error("Failed to initialize SDL");
	}
}

void Window::update() {
	SDL_GL_SwapWindow(window);
}


Window::~Window() {
	SDL_DestroyWindow(window);
}
