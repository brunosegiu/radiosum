#pragma once

#include <ctime>

#include "Camera.h"

class CameraController {
public:
	CameraController(SDL_Window* win, Camera* camera, float sensitivity, float speed);
	void update();
	virtual ~CameraController();
private:
	float sensitivity, speed;
	SDL_Window* win;
	int mouseX, mouseY;
	std::clock_t lastUpdate;
	bool in, set;
	Camera* camera;
};

