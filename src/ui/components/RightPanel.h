#pragma once

#include <imgui.h>

#include "Component.h"

class RightPanel : public Component {
public:
	RightPanel();
	void render();
	virtual ~RightPanel();
};