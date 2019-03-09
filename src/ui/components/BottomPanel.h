#pragma once

#include <imgui.h>

#include "Component.h"

class BottomPanel : public Component {
public:
	BottomPanel();
	void render();
	virtual ~BottomPanel();
};