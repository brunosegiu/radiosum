#include "Window.h"

#include "PreprocessingController.h"
#include "DisplayController.h"
#include "Scene.h"

int main(int argc, char *args[]) {
	Window *win = Window::get();
	Scene* world = new Scene();
	world->addMesh("mesh.obj");
	PreprocessingController* pre = new PreprocessingController(world);
	DisplayController* dis = new DisplayController(world);
	while (true) {
		pre->runStep();
		dis->render();
		win->update();
	}
	delete win;
	return 0;
}