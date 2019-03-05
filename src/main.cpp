#define GLM_FORCE_RADIANS

#include <Windows.h>

#include "common/Application.h"

int main(int argc, char *args[]) {
	try {
		Application::run();
	}
	catch (std::runtime_error &error) {
		MessageBox(NULL, error.what(), "Error", MB_ICONERROR);

	}
	return 0;
}
