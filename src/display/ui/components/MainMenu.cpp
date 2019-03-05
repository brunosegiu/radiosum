#include "display/ui/components/MainMenu.h"

#include <string>

#include "common/Application.h"
#include "common/Utils.h"

MainMenu::MainMenu() {
	this->preprocess = new Preprocess();
}

void MainMenu::render() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Load")) {
				std::string pathToObj = selectFile();
				if (pathToObj != "")
					Application::getScene()->addMesh(pathToObj);
			}
			ImGui::MenuItem("Save");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Preprocess")) {
			if (ImGui::MenuItem("Compute radiosity")) {
				this->preprocess->enable();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug")) {
			ImGui::MenuItem("Preprocess render", nullptr, true);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	this->preprocess->render();
}

MainMenu::~MainMenu() {
	delete this->preprocess;
}