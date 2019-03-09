#include "components/right/Preprocess.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "UIStore.h"
#include "EngineStore.h"

Preprocess::Preprocess() : Component() {
	this->enable();
}

void Preprocess::render() {
	if (enabled && ImGui::BeginTabItem("Preprocess")) {
		float progress = EngineStore::progress;
		ImGui::Spacing();
		ImGui::Text("Progress");
		ImGui::ProgressBar(progress);
		if (ImGui::Button("Compute form factors")) {
			UIStore::engine->preprocess();
		}
		if (progress != 1.0f) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		if (ImGui::Button("Compute radiosity")) {
			UIStore::engine->computeRadiosity();
		}
		if (progress != 1.0f) {
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
		ImGui::EndTabItem();
	}
}


Preprocess::~Preprocess() {
}
