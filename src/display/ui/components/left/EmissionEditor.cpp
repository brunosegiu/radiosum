#include "display/ui/components/left/EmissionEditor.h"

#include "common/Application.h"

EmissionEditor::EmissionEditor() : Component() {
	this->emission = .0f;
	this->current = 0;
	Application::attachListener(this);
	this->enable();
}

void EmissionEditor::render() {
	mousePos = ImGui::GetMousePos();
	mousePos = ImVec2(mousePos.x - (this->appWidth - this->width) / 2.0f, mousePos.y - 20.0f);
	if (enabled && ImGui::BeginTabItem("Emissions")) {
		if (ImGui::BeginChild("EmissionsWin", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight()))) {
			ImGui::Text(std::string("Face: " + std::to_string(current)).c_str());
			ImGui::SliderFloat("Emission", &this->emission, 0.0f, 1.0f);
			ImGui::Text(std::string("X: " + std::to_string(GLint(mousePos.x))).c_str());
			ImGui::SameLine();
			ImGui::Text(std::string("Y: " + std::to_string(GLint(mousePos.y))).c_str());
			if (ImGui::Button("Save")) {
				if (current > 0)
					Application::getScene()->setEmission(current - 1, this->emission);
			}
			ImGui::EndChild();
		}
		ImGui::EndTabItem();
	}
}


void EmissionEditor::process(SDL_Event &event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		GLuint picked = picker.getSelected(mousePos.x, mousePos.y);
		if (picked > 0) {
			this->current = picked;
			if (this->current > 0)
				this->emission = Application::getScene()->getEmission(current - 1);
		}

	}
}


EmissionEditor::~EmissionEditor() {
}
