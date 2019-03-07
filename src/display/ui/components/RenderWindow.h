#pragma once

#include <GL/glew.h>

#include "display/ui/Component.h"

class RenderWindow : public Component {
public:
	RenderWindow();
	void setTexture(GLuint texture);
	void render();
	virtual ~RenderWindow();
private:
	GLuint texture;
};

