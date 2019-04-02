#include "components/bottom/DebugInfo.h"

#include "imgui.h"

#include "ConfigurationManager.h"
#include "EngineStore.h"
#include "UIStore.h"

DebugInfo::DebugInfo() {
  this->appRes = ImVec2(std::stoi(ConfigurationManager::get("APP_RES_WIDTH")),
                        std::stoi(ConfigurationManager::get("APP_RES_HEIGHT")));
}

void DebugInfo::render() {
  if (ImGui::BeginTabItem("Log")) {
    if (ImGui::BeginChild("LogWin", ImVec2(ImGui::GetWindowContentRegionWidth(),
                                           ImGui::GetWindowHeight() - 40))) {
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
}

DebugInfo::~DebugInfo() {}
