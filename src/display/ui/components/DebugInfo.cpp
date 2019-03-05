#include "display/ui/components/DebugInfo.h"

#include "imgui.h"

#include "common/Logger.h"
#include "common/ConfigurationManager.h"


DebugInfo::DebugInfo() {
	this->appRes = ImVec2(std::stoi(ConfigurationManager::get("APP_RES_WIDTH")), std::stoi(ConfigurationManager::get("APP_RES_HEIGHT")));
}

void DebugInfo::render() {
	ImGui::SetNextWindowBgAlpha(0.1f);
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(appRes.x * .3f, appRes.y * .5f));
	ImGui::Begin("Log", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize(ImVec2(400, 0));
	ImVec2 deb = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(appRes.x - deb.x - X_OFFSET, appRes.y - deb.y - Y_OFFSET));
	std::vector<std::string> log = Logger::getLog();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Log:");
	for (std::string line : log) {
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "> ");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::TextWrapped(line.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::End();
	ImGui::Begin("Vars", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize(ImVec2(0, 0));
	deb = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(appRes.x - appRes.x - X_OFFSET, Y_OFFSET));
	std::map<std::string, std::string> vars = Logger::getVars();
	for (auto var : vars) {
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (var.first + ":").c_str());
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), var.second.c_str());
	}
	ImGui::End();
}


DebugInfo::~DebugInfo() {
}
