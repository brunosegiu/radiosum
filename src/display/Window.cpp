#include "display/Window.h"

#include "common/ConfigurationManager.h"

Window* Window::instance = nullptr;

Window* Window::get() {
	if (!Window::instance) {
		Window::instance = new Window();
	}
	return Window::instance;
}

Window::Window() {
	this->width = std::stoi(ConfigurationManager::get("APP_RES_WIDTH"));
	this->height = std::stoi(ConfigurationManager::get("APP_RES_HEIGHT"));
	this->name = ConfigurationManager::get("APP_NAME");
	this->isOpen = true;
	if (SDL_Init(SDL_INIT_VIDEO) >= 0) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		this->window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL) throw std::runtime_error("Failed to initialize SDL");
		glContext = SDL_GL_CreateContext(window);
		if (glContext == NULL)
			throw std::runtime_error("Failed to initialize OpenGL");
		glewExperimental = GL_TRUE;
		glewInit();

		// Load OpenGL setting
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);

		this->camera = new CameraController(window);
		this->listeners.push_back(this->camera);
	}
	else {
		throw std::runtime_error("Failed to initialize SDL");
	}
}

void Window::update() {
	this->camera->update();
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		for (auto listener : this->listeners)
			listener->process(event);

		if (event.type == SDL_QUIT)
			this->isOpen = false;
	}
	SDL_GL_SwapWindow(window);
}

bool Window::open() {
	return this->isOpen;
}

Camera* Window::getCamera() {
	return this->camera->getCamera();
}


Window::~Window() {
	delete this->camera;
	SDL_DestroyWindow(window);
}
