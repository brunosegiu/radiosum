#define GLM_FORCE_RADIANS

#include <Windows.h>
#include <thread>
#include <mutex>

#include "ui/Application.h"
#include "engine/Engine.h"

Application* app;
Engine* engine;

std::mutex mutexApp;
std::condition_variable waitForApp;
std::unique_lock<std::mutex> lockApp(mutexApp);

void startApp() {
	app = new Application();
	waitForApp.notify_one();
	engine = new Engine();
	//waitForEngine.wait(lockEngine);
	app->setEngine(engine);
	app->loop();
}

void startEngine() {
	//waitForApp.wait(lockApp);
	//waitForEngine.notify_one();
	//engine->loop();
}

int main(int argc, char *args[]) {
	try {
		std::thread appThread(startApp);
		std::thread engineThread(startEngine);
		appThread.join();
		engineThread.join();
	}
	catch (std::runtime_error &error) {
		MessageBox(NULL, error.what(), "Error", MB_ICONERROR);
	}

	return 0;
}
