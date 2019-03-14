#include "components/right/LightingEditor.h"

#include "Application.h"
#include "UIStore.h"

EmissionEditor::EmissionEditor() : Component() {
	this->emission = .0f;
	this->current = 0;
	this->enable();
}

void EmissionEditor::render() {
	mousePos = ImGui::GetMousePos();
	mousePos = ImVec2((1.0f / GLfloat(this->scale)) * (mousePos.x - (this->appWidth - this->width) / 2.0f), (1.0f / GLfloat(this->scale)) *  (mousePos.y - 20.0f));
	this->update();
	if (enabled && ImGui::BeginTabItem("Lighting")) {
		if (ImGui::BeginChild("LightingWin", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight()))) {
			ImGui::Text(std::string("Face: " + std::to_string(int(current) - 1)).c_str());

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
      
			ImGui::SliderFloat("Emission", &this->emission, 0.0f, 1.0f);
			if (ImGui::Button("Save emission")) {
				if (current > 0)
					UIStore::engine->getScene()->setEmission(current - 1, this->emission);
			}

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::ColorPicker3("Reflactance", this->reflactance);
			if (ImGui::Button("Save reflactance")) {
				if (current > 0)
					UIStore::engine->getScene()->setReflactance(current - 1, glm::vec3(this->reflactance[0], this->reflactance[1], this->reflactance[2]));
			}

			ImGui::Text(std::string("X: " + std::to_string(GLint(mousePos.x))).c_str());
			ImGui::SameLine();
			ImGui::Text(std::string("Y: " + std::to_string(GLint(mousePos.y))).c_str());
			ImGui::EndChild();
		}
		ImGui::EndTabItem();
	}
}


void EmissionEditor::update() {
	if (ImGui::IsMouseDoubleClicked(0)) {
		GLuint picked = UIStore::engine->pick(mousePos.x, mousePos.y);
		if (picked > 0) {
			this->current = picked;
			if (this->current > 0) {
				this->emission = UIStore::engine->getScene()->getEmission(current - 1);
				glm::vec3 ref = UIStore::engine->getScene()->getReflactance(current - 1);
				this->reflactance[0] = ref.x;
				this->reflactance[1] = ref.y;
				this->reflactance[2] = ref.z;
			}
		}

	}
}


EmissionEditor::~EmissionEditor() {
}
