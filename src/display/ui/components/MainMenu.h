#pragma once

#include <imgui.h>

#include "display/ui/Component.h"

class MainMenu : public Component {
public:
	MainMenu();
	void render();
	virtual ~MainMenu();
};