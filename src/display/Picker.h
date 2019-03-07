#pragma once

#include <GL/glew.h>

class Picker {
public:
	Picker();
	GLuint getSelected(GLint x, GLint y);
	virtual ~Picker();
};

