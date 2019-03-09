#pragma once

#include <imgui.h>

#include "Component.h"

class MainMenu : public Component {
public:
	MainMenu();
	void render();
	virtual ~MainMenu();
};