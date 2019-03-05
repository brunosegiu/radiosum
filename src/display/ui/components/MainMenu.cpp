#include "display/ui/components/MainMenu.h"

#include <string>
#include <windows.h>

#include "common/Application.h"

MainMenu::MainMenu() {
	this->preprocess = new Preprocess();
}

std::string getFile() {
	// From: https://www.daniweb.com/programming/software-development/code/217307/a-simple-getopenfilename-example
	OPENFILENAME ofn;
	char szFile[100];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
	return ofn.lpstrFile;
}

void MainMenu::render() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Load")) {
				std::string pathToObj = getFile();
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