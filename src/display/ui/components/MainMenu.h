#pragma once

#include <imgui.h>

#include "display/ui/Component.h"
#include "display/ui/components/Preprocess.h"

class MainMenu : public Component {
public:
	MainMenu();
	void render();
	virtual ~MainMenu();
private:
	Component* preprocess;
};