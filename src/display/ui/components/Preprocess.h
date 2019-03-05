#pragma once

#include <imgui.h>

#include "display/ui/Component.h"

class Preprocess : public Component {
public:
	Preprocess();
	void render();
	virtual ~Preprocess();
};