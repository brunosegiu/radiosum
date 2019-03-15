#include "Application.h"

#include "UIStore.h"


Application::Application() {
	this->window = new Window();
	this->sdlWin = this->window->getSDLWindow();
	this->glContext = this->window->getGlContext();

	this->ui = new UI(this->sdlWin, this->glContext);

	this->cameraController = new CameraController(this->sdlWin);

	this->attachListener(this->ui);
	this->attachListener(this->window);
	this->attachListener(this->cameraController);
}

void Application::update() {
	SDL_Event event;
	do {
		for (auto &listener : this->listeners)
			listener->process(event);
	} while (SDL_PollEvent(&event) != 0);
}

void Application::setEngine(Engine* engine) {
	this->engine = engine;
	UIStore::engine = this->engine;
}

void Application::attachListener(EventListener* listener) {
	this->listeners.push_back(listener);
}

void Application::loop() {
	do {
		this->update();
		this->engine->step();
		this->ui->setTexture(this->engine->outputTexture);
		this->ui->render();
		SDL_GL_SwapWindow(this->sdlWin);
	} while (this->window->open());
}

Application::~Application() {
	delete this->window;
}
