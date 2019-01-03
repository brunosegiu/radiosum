#pragma once

#include <ctime>

#include "Camera.h"

class CameraController {
public:
	CameraController(SDL_Window* win, Camera* camera = nullptr, float sensitivity = 50.0f, float speed = 50.0f);
	void update();
	void toggleIn();
	virtual ~CameraController();
private:
	float sensitivity, speed;
	SDL_Window* win;
	int mouseX, mouseY;
	std::clock_t lastUpdate;
	bool in, set;
	Camera* camera;
	GLuint width, height;
};

