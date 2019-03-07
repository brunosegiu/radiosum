#include "CameraController.h"

#define _USE_MATH_DEFINES

#include <stdexcept>
#include <cmath> 

#include <GL/glew.h>
#include <SDL.h>
#include <glm.hpp>
#include <gtx/rotate_vector.hpp>

#include "common/ConfigurationManager.h"
#include "common/Application.h"

CameraController::CameraController(SDL_Window* win, Camera* camera) {
	if (!win) {
		throw std::runtime_error("No window object to wrap camera to");
	}
	else {
		if (camera == nullptr)
			camera = new Camera(1.0f, 90.0f, 0.5f, 5000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->camera = camera;
		this->sensitivity = std::stoi(ConfigurationManager::get("MOUSE_SENSITIVITY"));
		this->speed = std::stoi(ConfigurationManager::get("MOUSE_SPEED"));
		this->in = false;
		this->set = false;
		this->win = win;
		this->lastUpdate = std::clock();
		int width, height;
		SDL_GetWindowSize(win, &width, &height);
		this->width = GLuint(width);
		this->height = GLuint(height);
	}
}

void CameraController::update() {
	glm::vec3 origin = this->camera->getOrigin();
	glm::vec3 direction = this->camera->getDirection();
	glm::vec3 up = this->camera->getUp();

	if (!set) {
		set = true;
		SDL_WarpMouseInWindow(win, unsigned int(width / 2), unsigned int(height / 2));
	}

	if (in) {
		float duration = float((std::clock() - lastUpdate) / (double)CLOCKS_PER_SEC);

		const Uint8* state = SDL_GetKeyboardState(NULL);

		glm::vec3 reference = origin + direction;
		glm::vec3 right = glm::cross(direction, up);

		// Move according to key presses
		if (state[SDL_SCANCODE_W]) {
			glm::vec3 fact = speed * duration * direction;
			origin += fact;
			reference += fact;
		}
		else if (state[SDL_SCANCODE_S]) {
			glm::vec3 fact = speed * duration * direction;
			origin -= fact;
			reference -= fact;
		}
		if (state[SDL_SCANCODE_A]) {
			glm::vec3 fact = speed * duration * right;
			origin -= fact;
			reference -= fact;
		}
		else if (state[SDL_SCANCODE_D]) {
			glm::vec3 fact = speed * duration * right;
			origin += fact;
			reference += fact;
		}

		direction = glm::normalize(reference - origin);
		right = glm::cross(direction, up);

		// Move according to mouse presses
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x != mouseX || y != mouseY) {
			float dirX, dirY;
			dirX = sensitivity * duration * (mouseX - x) / width;
			dirY = sensitivity * duration * (mouseY - y) / height; // Inverted
			direction = glm::mat3(glm::rotate(dirX, up)) * direction;
			direction = glm::mat3(glm::rotate(dirY, right)) * direction;
		}

		// Update Camera
		this->camera->setOrigin(origin);
		this->camera->setDirection(direction);

		SDL_WarpMouseInWindow(win, unsigned int(width / 2), unsigned int(height / 2));
		mouseX = unsigned int(width / 2);
		mouseY = unsigned int(height / 2);
	}
	this->lastUpdate = clock();
}

void CameraController::process(SDL_Event &event) {
	switch (event.type) {
	case SDL_KEYDOWN: {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			this->in = false;
			SDL_ShowCursor(SDL_ENABLE);
		}
		else if (event.key.keysym.sym == SDLK_F11) {
			this->toggleIn();
		}
	} break;
	}
}

Camera* CameraController::getCamera() {
	return this->camera;
}

void CameraController::toggleIn() {
	this->in = !this->in;
	if (in) {
		SDL_ShowCursor(SDL_DISABLE);
	}
	else {
		SDL_ShowCursor(SDL_ENABLE);
	}
}

CameraController::~CameraController() {
	delete this->camera;
}
