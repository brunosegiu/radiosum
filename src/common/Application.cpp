#include "common/Application.h"

#include "common/Logger.h"
#include "common/buffers/PickingBuffer.h"

Application* Application::instance = nullptr;

Application* Application::getInstance() {
	if (Application::instance == nullptr) {
		Application::instance = new Application();
	}
	return Application::instance;
}

std::vector<EventListener*> Application::listeners;
void Application::attachListener(EventListener* listener) {
	Application::listeners.push_back(listener);
}

std::vector<EventListener*> Application::getEventListeners() {
	return Application::listeners;
}

Application::Application() {
	this->window = nullptr;
	this->currentScene = nullptr;
	this->displayer = nullptr;
	this->preprocessor = nullptr;
	this->sceneRenderer = nullptr;
	this->mode = DISPLAY_FACES;
	this->step = 0;
}

void Application::init() {
	Application* app = Application::getInstance();

	app->window = new Window();
	app->currentScene = new Scene();
	app->displayer = new DisplayController(app->mode);
	app->sceneRenderer = new SceneRenderer();
	app->sceneRenderer->setScene(app->currentScene);

	app->sdlWin = app->window->getSDLWindow();
	app->glContext = app->window->getGlContext();

	app->ui = new UI(app->sdlWin, app->glContext);

	Application::attachListener(app->ui);
	Application::attachListener(app->window);
}

void Application::update() {
	SDL_Event event;
	do {
		for (auto listener : Application::getEventListeners())
			listener->process(event);
	} while (SDL_PollEvent(&event) != 0);
}

void Application::run() {
	Application* app = Application::getInstance();
	do {
		app->update();
		if (app->mode == PREPROCESS && app->preprocessor != nullptr) {
			app->step = app->preprocessor->runStep();
		}
		else if (app->mode != PREPROCESS) {
			app->sceneRenderer->render();
			app->sceneRenderer->read();
		}
		app->displayer->render();
		GLuint texture = app->displayer->read();
		app->ui->setTexture(texture);
		app->ui->render();
		SDL_GL_SwapWindow(app->sdlWin);
	} while (app->window->open());
}

RunMode Application::getMode() {
	Application* app = Application::getInstance();
	return app->mode;
}

void Application::setMode(RunMode mode) {
	Application* app = Application::getInstance();
	if (mode != app->mode) {
		app->mode = mode;
		app->displayer->setMode(mode);
	}
}

float Application::getProgress() {
	Application* app = Application::getInstance();
	if (app->currentScene && app->currentScene->size() > 0)
		return app->step / float(app->currentScene->size());
	return 0.0f;
}

Scene* Application::getScene() {
	Application* app = Application::getInstance();
	return app->currentScene;
}

Window* Application::getWindow() {
	Application* app = Application::getInstance();
	return app->window;
}

PickingBuffer* Application::getPickingBuffer() {
	Application* app = Application::getInstance();
	return dynamic_cast<PickingBuffer*>(app->sceneRenderer->getBuffer());
}

void Application::startPreprocessPass() {
	Logger::log("Starting preprocess pass");
	Application* app = Application::getInstance();
	if (app->preprocessor)
		delete app->preprocessor;
	app->preprocessor = new PreprocessingController(app->currentScene);
	Application::setMode(PREPROCESS);
}

void Application::computeRadiosity() {
	Application* app = Application::getInstance();
	if (app->currentScene && app->preprocessor && app->step == app->currentScene->size())
		app->preprocessor->computeRadiosity();
}


Application::~Application() {
	delete this->window;
}
