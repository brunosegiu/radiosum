#include "components/left/ReflactanceEditor.h"

#include "Application.h"
#include "UIStore.h"

ReflactanceEditor::ReflactanceEditor() : Component() {
	this->reflactance = .0f;
	this->current = 0;
	this->enable();
}

void ReflactanceEditor::render() {
	mousePos = ImGui::GetMousePos();
	mousePos = ImVec2(mousePos.x - (this->appWidth - this->width) / 2.0f, mousePos.y - 20.0f);
	this->update();
	if (enabled && ImGui::BeginTabItem("Reflactance")) {
		if (ImGui::BeginChild("ReflactancesWin", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight()))) {
			ImGui::Text(std::string("Face: " + std::to_string(int(current) - 1)).c_str());
			ImGui::SliderFloat("Reflactance", &this->reflactance, 0.0f, 1.0f);
			ImGui::Text(std::string("X: " + std::to_string(GLint(mousePos.x))).c_str());
			ImGui::SameLine();
			ImGui::Text(std::string("Y: " + std::to_string(GLint(mousePos.y))).c_str());
			if (ImGui::Button("Save")) {
				if (current > 0)
					UIStore::engine->getScene()->setReflactance(current - 1, this->reflactance);
			}
			ImGui::EndChild();
		}
		ImGui::EndTabItem();
	}
}


void ReflactanceEditor::update() {
	if (ImGui::IsMouseDoubleClicked(0)) {
		GLuint picked = UIStore::engine->pick(mousePos.x, mousePos.y);
		if (picked > 0) {
			this->current = picked;
			if (this->current > 0)
				this->reflactance = UIStore::engine->getScene()->getReflactance(current - 1);
		}

	}
}


ReflactanceEditor::~ReflactanceEditor() {
}
