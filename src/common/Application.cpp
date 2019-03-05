#include "common/Application.h"

#include "common/Logger.h"

Application* Application::instance = nullptr;

Application* Application::getInstance() {
	if (Application::instance == nullptr)
		Application::instance = new Application();
	return Application::instance;
}

Application::Application() {
	this->win = Window::get();
	this->currentScene = new Scene();
	this->displayer = new DisplayController();
	this->preprocessor = nullptr;
	this->sceneRenderer = new SceneRenderer();
	this->sceneRenderer->setScene(currentScene);
	this->mode = DISPLAY;
}

void Application::run() {
	Application* app = Application::getInstance();
	while (app->win->open()) {
		app->win->update();
		if (app->mode == PREPROCESS && app->preprocessor != nullptr) {
			GLuint step = app->preprocessor->runStep();
		}
		else if (app->mode == DISPLAY) {
			app->sceneRenderer->render();
			app->sceneRenderer->read();
		}
		app->displayer->render();
	}

}

void Application::setMode(RunMode mode) {
	Application* app = Application::getInstance();
	app->mode = mode;
	app->displayer->setMode(mode);
}

Scene*  Application::getScene() {
	Application* app = Application::getInstance();
	return app->currentScene;
}

void Application::startPreprocessPass() {
	Logger::log("Starting preprocess pass");
	Application* app = Application::getInstance();
	app->preprocessor = new PreprocessingController(app->currentScene);
	Application::setMode(PREPROCESS);
}


Application::~Application() {
}
