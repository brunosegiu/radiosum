#pragma once

#include <imgui.h>

inline void renderMenu() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Scene")) {
			ImGui::MenuItem("Load");
			ImGui::MenuItem("Save");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Renderer"))
		{
			ImGui::MenuItem("Compute radiosity");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}