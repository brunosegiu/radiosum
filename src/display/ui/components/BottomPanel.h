#pragma once

#include <imgui.h>

#include "display/ui/Component.h"

class BottomPanel : public Component {
public:
	BottomPanel();
	void render();
	virtual ~BottomPanel();
};