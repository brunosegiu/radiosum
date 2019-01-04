#pragma once

#include <ctime>

#include "Camera.h"
#include "EventListener.h"

class CameraController : public EventListener {
public:
	CameraController(SDL_Window* win, Camera* camera = nullptr);
	void update();
	void toggleIn();
	void process(SDL_Event &event);

	Camera* getCamera();

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

