#include "components/LeftPanel.h"
#include "components/left/SceneEditor.h"

#include "imgui.h"

LeftPanel::LeftPanel() : Component() {
  this->enable();
  this->children["EDITOR"] = new SceneEditor();
}

void LeftPanel::render() {
  ImGui::SetNextWindowSize(ImVec2((this->appWidth - this->width) / 2,
                                  float(this->appHeight) - 20.0f));
  ImGui::SetNextWindowPos(ImVec2(.0f, 20.0f));
  if (enabled &&
      ImGui::Begin("GeometryWindow", nullptr,
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoDecoration)) {
    if (ImGui::BeginTabBar("Left")) {
      for (auto &component : this->children) {
        component.second->render();
      }
      ImGui::EndTabBar();
    }
    ImGui::End();
  }
}

LeftPanel::~LeftPanel() {}
