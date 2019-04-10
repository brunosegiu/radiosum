#include "components/right/LightingEditor.h"

#include "EngineStore.h"
#include "UIStore.h"

LightingEditor::LightingEditor() : Component() {
  this->emission = .0f;
  this->enable();
  this->lastSelected = 0;
}

void LightingEditor::render() {
  if (UIStore::selectedFace > 0 && lastSelected != UIStore::selectedFace) {
    this->lastSelected = UIStore::selectedFace;
    this->emission =
        UIStore::engine->getScene()->getEmission(UIStore::selectedFace);
    glm::vec3 ref =
        UIStore::engine->getScene()->getReflactance(UIStore::selectedFace);
    this->reflactance[0] = ref.x;
    this->reflactance[1] = ref.y;
    this->reflactance[2] = ref.z;
  }

  if (enabled && ImGui::BeginTabItem("Lighting")) {
    if (ImGui::BeginChild("LightingWin",
                          ImVec2(ImGui::GetWindowContentRegionWidth(),
                                 ImGui::GetWindowHeight()))) {
      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::Spacing();

      ImGui::SliderFloat("Emission", &this->emission, 0.0f, 100.0f);
      if (ImGui::Button("Save emission")) {
        if (UIStore::selectedFace >= 0)
          UIStore::engine->getScene()->setEmission(
              UIStore::selectedFace, this->emission, !UIStore::selectFace);
      }

      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::Spacing();

      ImGui::ColorPicker3("Reflactance", this->reflactance);
      if (ImGui::Button("Save reflactance")) {
        if (UIStore::selectedFace >= 0)
          UIStore::engine->getScene()->setReflactance(
              UIStore::selectedFace,
              glm::vec3(this->reflactance[0], this->reflactance[1],
                        this->reflactance[2]),
              !UIStore::selectFace);
      }

      ImGui::SliderFloat("Radiosity scale", &EngineStore::radiosityScale, .0f,
                         10.0f);

      ImGui::EndChild();
    }
    ImGui::EndTabItem();
  }
}

LightingEditor::~LightingEditor() {}
