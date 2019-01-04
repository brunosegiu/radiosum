#include "Window.h"

#include "PreprocessingController.h"
#include "DisplayController.h"
#include "Mesh.h"

int main(int argc, char *args[]) {
	Window *win = Window::get();
	Mesh* mesh = Mesh::load("mesh.obj");
	std::vector<Mesh*> meshes;
	meshes.push_back(mesh);
	PreprocessingController* pre = new PreprocessingController(meshes);
	DisplayController* dis = new DisplayController();
	while (true) {
		pre->runStep();
		dis->render();
		win->update();
	}
	delete win;
	return 0;
}