#include "components/left/EmissionEditor.h"

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
	if (enabled && ImGui::BeginTabItem("Emissions")) {
		if (ImGui::BeginChild("EmissionsWin", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight()))) {
			ImGui::Text(std::string("Face: " + std::to_string(int(current) - 1)).c_str());
			ImGui::SliderFloat("Emission", &this->emission, 0.0f, 10.0f);
			ImGui::Text(std::string("X: " + std::to_string(GLint(mousePos.x))).c_str());
			ImGui::SameLine();
			ImGui::Text(std::string("Y: " + std::to_string(GLint(mousePos.y))).c_str());
			if (ImGui::Button("Save")) {
				if (current > 0)
					UIStore::engine->getScene()->setEmission(current - 1, this->emission);
			}
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
			if (this->current > 0)
				this->emission = UIStore::engine->getScene()->getEmission(current - 1);
		}

	}
}


EmissionEditor::~EmissionEditor() {
}
