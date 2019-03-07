#include "display/ui/components/RightPanel.h"

#include "imgui.h"

#include "common/Application.h"
#include "display/ui/components/right/Preprocess.h"

RightPanel::RightPanel() : Component() {
	this->enable();
	this->children["PREPROCESS"] = new Preprocess();
}

void RightPanel::render() {
	ImGui::SetNextWindowSize(ImVec2((this->appWidth - this->width) / 2, float(this->appHeight) - 20.0f));
	ImGui::SetNextWindowPos(ImVec2(this->width + (this->appWidth - this->width) / 2, 20.0f));
	if (enabled && ImGui::Begin("LightingWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration)) {
		if (ImGui::BeginTabBar("Right")) {
			for (auto &component : this->children) {
				component.second->render();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

RightPanel::~RightPanel() {
}
