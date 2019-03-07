#pragma once

#include <imgui.h>

#include "display/ui/Component.h"

class LeftPanel : public Component {
public:
	LeftPanel();
	void render();
	virtual ~LeftPanel();
};