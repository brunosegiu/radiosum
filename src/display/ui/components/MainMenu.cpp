#include "display/ui/components/MainMenu.h"

#include <string>

#include "common/Application.h"
#include "common/Utils.h"

MainMenu::MainMenu() {
}

void MainMenu::render() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("Load mesh")) {
					std::string pathToObj = selectFile();
					if (pathToObj != "")
						Application::getScene()->addMesh(pathToObj);
				}
				ImGui::MenuItem("Export");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::BeginMenu("Debug")) {
				ImGui::MenuItem("Render preprocess pass", nullptr, true);
				ImGui::MenuItem("Logging", nullptr, true);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Render")) {
				if (ImGui::MenuItem("Radiosity", nullptr, Application::getMode() == DISPLAY_RADIOSITY)) {
					Application::setMode(DISPLAY_RADIOSITY);
				}
				if (ImGui::MenuItem("Emission", nullptr, Application::getMode() == DISPLAY_EMISSION)) {
					Application::setMode(DISPLAY_EMISSION);
				}
				if (ImGui::MenuItem("Faces", nullptr, Application::getMode() == DISPLAY_FACES)) {
					Application::setMode(DISPLAY_FACES);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

MainMenu::~MainMenu() {
}