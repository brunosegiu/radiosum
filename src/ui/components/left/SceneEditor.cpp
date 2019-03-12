#include "components/left/SceneEditor.h"

SceneEditor::SceneEditor() : Component() {
	this->enable();
}

void SceneEditor::render() {
	if (enabled && ImGui::BeginTabItem("Geometry")) {
		if (ImGui::BeginChild("GeometryWin", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight()))) {
			ImGui::EndTabItem();
		}
	}
}


SceneEditor::~SceneEditor() {
}
