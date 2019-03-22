#include "Window.h"

#include "ConfigurationManager.h"
#include "UIStore.h"
#include "Application.h"

Window::Window() {
	this->width = std::stoi(ConfigurationManager::get("APP_RES_WIDTH"));
	this->height = std::stoi(ConfigurationManager::get("APP_RES_HEIGHT"));
	this->name = ConfigurationManager::get("APP_NAME");
	this->isOpen = true;
	if (SDL_Init(SDL_INIT_VIDEO) >= 0) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		this->window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL) throw std::runtime_error("Failed to initialize SDL");
		glContext = SDL_GL_CreateContext(window);
		if (glContext == NULL)
			throw std::runtime_error("Failed to initialize OpenGL");
		glewExperimental = GL_TRUE;
		glewInit();

		// Load OpenGL settings
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		throw std::runtime_error("Failed to initialize SDL");
	}
}

void Window::process(SDL_Event &event) {
	if (event.type == SDL_QUIT)
		this->isOpen = false;
}

bool Window::open() {
	return this->isOpen;
}

SDL_Window* Window::getSDLWindow() {
	return this->window;
}

SDL_GLContext Window::getGlContext() {
	return this->glContext;
}

Window::~Window() {
	SDL_DestroyWindow(window);
}
