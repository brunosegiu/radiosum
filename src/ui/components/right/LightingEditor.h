#pragma once

#include <GL/glew.h>

#include "Component.h"
#include "EventListener.h"

class EmissionEditor : public Component {
public:
	EmissionEditor();
	void render();
	void update();
	virtual ~EmissionEditor();
private:
	GLfloat emission, reflactance;
	GLuint current;
	ImVec2 mousePos;
};

