#pragma once

#include <GL/glew.h>

#include "display/ui/Component.h"
#include "display/Picker.h"
#include "display/EventListener.h"

class EmissionEditor : public Component, public EventListener {
public:
	EmissionEditor();
	void render();
	void process(SDL_Event &event);
	virtual ~EmissionEditor();
private:
	Picker picker;
	GLfloat emission;
	GLuint current;
	ImVec2 mousePos;
};

