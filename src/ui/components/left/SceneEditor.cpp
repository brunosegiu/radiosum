#include "components/left/SceneEditor.h"
#include "UIStore.h"

SceneEditor::SceneEditor() : Component() {
  this->enable();
  this->prevSelectedFace = 0;
}

void SceneEditor::render() {
  if (enabled && ImGui::BeginTabItem("Geometry")) {
    if (ImGui::BeginChild("GeometryWin",
                          ImVec2(ImGui::GetWindowContentRegionWidth(),
                                 ImGui::GetWindowHeight()))) {
      ImGui::Text("Selection mode");
      if (ImGui::RadioButton("Face", UIStore::selectFace)) {
        UIStore::selectFace = true;
      }
      ImGui::SameLine();
      ImGui::Text(std::to_string(int(UIStore::selectedFace)).c_str());
      ImGui::SameLine();
      if (ImGui::RadioButton("Object", !UIStore::selectFace)) {
        UIStore::selectFace = false;
      }

      ImGui::Text(
          ("Faces: " + std::to_string(UIStore::engine->getScene()->size()))
              .c_str());

      // ImGui::Text("Debug");
      // ImGui::SliderInt("Go to face", &UIStore::selectedFace, 0,
      //                 UIStore::engine->getScene()->size());
      // if (prevSelectedFace != UIStore::selectedFace &&
      //    UIStore::selectedFace < UIStore::engine->getScene()->size()) {
      //  prevSelectedFace = UIStore::selectedFace;
      //  Face selected =
      //      UIStore::engine->getScene()->getFace(UIStore::selectedFace);
      //  UIStore::camera->goTo(selected);
      //}
      ImGui::EndChild();
    }
    ImGui::EndTabItem();
  }
}

SceneEditor::~SceneEditor() {}
