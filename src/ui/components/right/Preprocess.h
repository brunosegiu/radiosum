#pragma once

#include <imgui.h>

#include "Component.h"

class Preprocess : public Component {
public:
	Preprocess();
	void render();
	virtual ~Preprocess();
private:
	bool smooth;
	int channelCount;
};