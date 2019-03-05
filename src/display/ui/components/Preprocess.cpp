#include "display/ui/components/Preprocess.h"

#include "imgui.h"

#include "common/Application.h"

Preprocess::Preprocess() : Component() {
}

void Preprocess::render() {
	if (enabled && ImGui::Begin("Preprocess", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
		ImGui::ProgressBar(.5f);
		if (ImGui::Button("Start")) {
			Application::startPreprocessPass();
			enabled = false;
		}
		ImGui::End();
	}
}


Preprocess::~Preprocess() {
}
