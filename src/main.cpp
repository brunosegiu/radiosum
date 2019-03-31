#define GLM_FORCE_RADIANS

#include <Windows.h>

#include "engine/Engine.h"
#include "ui/Application.h"

int main(int argc, char* args[]) {
  try {
    Application* app;
    Engine* engine;
    app = new Application();
    engine = new Engine();
    app->setEngine(engine);
    app->loop();
  } catch (std::runtime_error& error) {
    MessageBox(NULL, error.what(), "Error", MB_ICONERROR);
  }

  return 0;
}
