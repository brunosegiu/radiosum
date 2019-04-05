#include "components/right/LightingEditor.h"

#include "UIStore.h"

EmissionEditor::EmissionEditor() : Component() {
  this->emission = .0f;
  this->enable();
}

void EmissionEditor::render() {
  mousePos = ImGui::GetMousePos();
  mousePos = ImVec2((1.0f / GLfloat(this->scale)) *
                        (mousePos.x - (this->appWidth - this->width) / 2.0f),
                    (1.0f / GLfloat(this->scale)) * (mousePos.y - 20.0f));
  this->update();
  if (enabled && ImGui::BeginTabItem("Lighting")) {
    if (ImGui::BeginChild("LightingWin",
                          ImVec2(ImGui::GetWindowContentRegionWidth(),
                                 ImGui::GetWindowHeight()))) {
      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::Spacing();

      ImGui::SliderFloat("Emission", &this->emission, 0.0f, 1.0f);
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

      ImGui::Text(
          std::string("X: " + std::to_string(GLint(mousePos.x))).c_str());
      ImGui::SameLine();
      ImGui::Text(
          std::string("Y: " + std::to_string(GLint(mousePos.y))).c_str());
      ImGui::EndChild();
    }
    ImGui::EndTabItem();
  }
}

void EmissionEditor::update() {
  if (ImGui::IsMouseDoubleClicked(0)) {
    GLuint picked = UIStore::engine->pick(mousePos.x, mousePos.y);
    if (picked > 0) {
      UIStore::selectedFace = GLint(picked) - 1;
      if (UIStore::selectedFace > 0) {
        this->emission =
            UIStore::engine->getScene()->getEmission(UIStore::selectedFace);
        glm::vec3 ref =
            UIStore::engine->getScene()->getReflactance(UIStore::selectedFace);
        this->reflactance[0] = ref.x;
        this->reflactance[1] = ref.y;
        this->reflactance[2] = ref.z;
      }
    }
  }
}

EmissionEditor::~EmissionEditor() {}
