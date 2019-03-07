#pragma once

#include <imgui.h>

#include "display/ui/Component.h"

class RightPanel : public Component {
public:
	RightPanel();
	void render();
	virtual ~RightPanel();
};