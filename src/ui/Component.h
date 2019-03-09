#pragma once

#include <map>

#include <GL/glew.h>

#include <imgui.h>

class Component {
public:
	Component();
	virtual void render() = 0;
	void enable();
	virtual ~Component();
protected:
	bool enabled;
	GLuint width, height, appWidth, appHeight;
	std::map<std::string, Component*> children;
};

