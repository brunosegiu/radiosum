#define GLM_FORCE_RADIANS

#include "display/Window.h"
#include "display/DisplayController.h"
#include "preprocessing/PreprocessingController.h"
#include "common/geometry/Scene.h"

int main(int argc, char *args[]) {
	Window *win = Window::get();
	Scene* world = new Scene();
	world->addMesh("street.obj");
	PreprocessingController* pre = new PreprocessingController(world);
	DisplayController* dis = new DisplayController(world);
	while (win->open()) {
		pre->runStep();
		dis->render();
		win->update();
	}
	delete win;
	delete pre;
	delete dis;
	delete world;
	return 0;
}