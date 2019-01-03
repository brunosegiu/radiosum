#include "Window.h"

#include "PreprocessingController.h"
#include "Mesh.h"

int main(int argc, char *args[]) {
	Window *win = new Window(800, 600, "Radiosum");
	Mesh* mesh = Mesh::load("mesh.obj");
	std::vector<Mesh*> meshes;
	meshes.push_back(mesh);
	PreprocessingController* pre = new PreprocessingController(meshes);
	while (true) {
		pre->runStep();
		win->update();
	}
	delete win;
	return 0;
}