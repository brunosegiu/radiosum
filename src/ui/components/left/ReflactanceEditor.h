#pragma once

#include <GL/glew.h>

#include "Component.h"
#include "EventListener.h"

class ReflactanceEditor : public Component {
public:
	ReflactanceEditor();
	void render();
	void update();
	virtual ~ReflactanceEditor();
private:
	GLfloat reflactance;
	GLuint current;
	ImVec2 mousePos;
};

