#include "display/ui/components/BottomPanel.h"

#include "imgui.h"

#include "common/Application.h"
#include "display/ui/components/bottom/DebugInfo.h"

BottomPanel::BottomPanel() : Component() {
	this->enable();
	this->children["LOG"] = new DebugInfo();
}

void BottomPanel::render() {
	ImGui::SetNextWindowSize(ImVec2(this->width, float(this->appHeight) - float(this->height) - 20.0f));
	ImGui::SetNextWindowPos(ImVec2((this->appWidth - this->width) / 2, this->height + 20.0f));
	if (enabled && ImGui::Begin("LogWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration)) {
		if (ImGui::BeginTabBar("Bottom")) {
			for (auto &component : this->children) {
				component.second->render();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}


BottomPanel::~BottomPanel() {
}
