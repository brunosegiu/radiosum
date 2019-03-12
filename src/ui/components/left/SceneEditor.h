#pragma once

#include <GL/glew.h>

#include "Component.h"

class SceneEditor : public Component {
public:
	SceneEditor();
	void render();
	void update();
	virtual ~SceneEditor();
};

