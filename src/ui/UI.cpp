#include "UI.h"

#include "UIStore.h"
#include "Application.h"
#include "ConfigurationManager.h"

#include "components/MainMenu.h"
#include "components/LeftPanel.h"
#include "components/BottomPanel.h"
#include "components/RightPanel.h"

UI::UI(SDL_Window* window, SDL_GLContext &glContext) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 150");
	this->window = window;

	this->components["MENU"] = new MainMenu();
	this->externalComponent = new RenderWindow();
	this->components["LEFT"] = new LeftPanel();
	this->components["RIGHT"] = new BottomPanel();
	this->components["BOTTOM"] = new RightPanel();
}

void UI::process(SDL_Event &event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void UI::render() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	glClear(GL_COLOR_BUFFER_BIT);
	this->externalComponent->render();
	for (auto &component : components) {
		component.second->render();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::setTexture(GLuint texture) {
	this->externalComponent->setTexture(texture);
}

UI::~UI() {
	for (auto &component : components) {
		delete component.second;
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
