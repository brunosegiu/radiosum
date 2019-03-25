#include "components/MainMenu.h"

#include <string>

#include "UIStore.h"
#include "Utils.h"

MainMenu::MainMenu() {
}

void MainMenu::render() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("Load mesh")) {
					std::string pathToObj = selectFile();
					if (pathToObj != "")
						UIStore::engine->addMesh(pathToObj);
				}
				ImGui::MenuItem("Export");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::BeginMenu("Render")) {
				if (ImGui::MenuItem("Radiosity", nullptr, UIStore::engine->getMode() == RADIOSITY)) {
					UIStore::engine->setMode(RADIOSITY);
				}
				if (ImGui::MenuItem("Emission", nullptr, UIStore::engine->getMode() == EMISSION)) {
					UIStore::engine->setMode(EMISSION);
				}
				if (ImGui::MenuItem("Reflactance", nullptr, UIStore::engine->getMode() == REFLACTANCE)) {
					UIStore::engine->setMode(REFLACTANCE);
				}
				if (ImGui::MenuItem("Faces", nullptr, UIStore::engine->getMode() == FACES)) {
					UIStore::engine->setMode(FACES);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Render preprocess pass", nullptr, UIStore::enablePreprocessRendering)) {
					UIStore::enablePreprocessRendering = !UIStore::enablePreprocessRendering;
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