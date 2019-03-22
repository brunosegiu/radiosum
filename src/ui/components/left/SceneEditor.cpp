#include "components/left/SceneEditor.h"
#include "UIStore.h"

SceneEditor::SceneEditor() : Component() {
	this->enable();
}

void SceneEditor::render() {
	if (enabled && ImGui::BeginTabItem("Geometry")) {
		if (ImGui::BeginChild("GeometryWin", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight()))) {
			ImGui::Text("Selection mode");
			if (ImGui::RadioButton("Face", UIStore::selectFace)) {
				UIStore::selectFace = true;
			}
			if (ImGui::RadioButton("Object", !UIStore::selectFace)) {
				UIStore::selectFace = false;
			}
			ImGui::Text(("Faces: " + std::to_string(UIStore::engine->getScene()->size())).c_str());
			ImGui::EndChild();
		}
		ImGui::EndTabItem();
	}
}


SceneEditor::~SceneEditor() {
}
