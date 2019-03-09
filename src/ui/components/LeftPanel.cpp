#include "components/LeftPanel.h"

#include "imgui.h"

#include "components/left/EmissionEditor.h"
#include "components/left/ReflactanceEditor.h"

LeftPanel::LeftPanel() : Component() {
	this->enable();
	this->children["EMISSION"] = new EmissionEditor();
	this->children["REFLACTANCE"] = new ReflactanceEditor();
}

void LeftPanel::render() {
	ImGui::SetNextWindowSize(ImVec2((this->appWidth - this->width) / 2, float(this->appHeight) - 20.0f));
	ImGui::SetNextWindowPos(ImVec2(.0f, 20.0f));
	if (enabled && ImGui::Begin("GeometryWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration)) {
		if (ImGui::BeginTabBar("Left")) {
			for (auto &component : this->children) {
				component.second->render();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}


LeftPanel::~LeftPanel() {
}
