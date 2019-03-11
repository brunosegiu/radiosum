#include "components/bottom/DebugInfo.h"

#include "imgui.h"

#include "UIStore.h"
#include "EngineStore.h"
#include "ConfigurationManager.h"


DebugInfo::DebugInfo() {
	this->appRes = ImVec2(std::stoi(ConfigurationManager::get("APP_RES_WIDTH")), std::stoi(ConfigurationManager::get("APP_RES_HEIGHT")));
}

void DebugInfo::render() {
	if (ImGui::BeginTabItem("Log")) {
		if (ImGui::BeginChild("LogWin", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight()))) {
			std::vector<std::string> log = EngineStore::logger.getLog();
			for (std::string line : log) {
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "> ");
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				ImGui::TextWrapped(line.c_str());
				ImGui::PopStyleColor();
			}
			ImGui::SetScrollHereY();
			ImGui::EndChild();
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Stats")) {
		auto vars = UIStore::logger.getFloats();
		for (auto var : vars) {
			GLfloat max = FLT_MIN;
			GLfloat min = FLT_MAX;
			GLfloat avg = .0f;
			for (GLfloat value : var.second) {
				max = max > value ? max : value;
				min = min < value ? min : value;
				avg += value;
			}
			avg *= 1.0f / GLfloat(var.second.size());
			ImGui::PlotLines(var.first.c_str(), &var.second[0], var.second.size(), 0, std::string("avg " + std::to_string(avg) + "s").c_str(), min, max);
		}
		ImGui::EndTabItem();
	}
}


DebugInfo::~DebugInfo() {
}
