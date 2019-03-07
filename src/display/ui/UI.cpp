#include "display/ui/UI.h"

#include "common/Logger.h"
#include "common/Application.h"
#include "common/ConfigurationManager.h"

#include "display/ui/components/MainMenu.h"
#include "display/ui/components/RenderWindow.h"
#include "display/ui/components/LeftPanel.h"
#include "display/ui/components/BottomPanel.h"
#include "display/ui/components/RightPanel.h"

UI::UI(SDL_Window* window, SDL_GLContext &glContext) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	Logger::log("Setting up ImGui");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 150");
	this->window = window;

	this->components["MENU"] = new MainMenu();
	this->components["RENDER"] = new RenderWindow();
	this->components["LEFT"] = new LeftPanel();
	this->components["RIGHT"] = new BottomPanel();
	this->components["BOTTOM"] = new RightPanel();
}

void UI::process(SDL_Event &event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void UI::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	for (auto &component : components) {
		component.second->render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::setTexture(GLuint texture) {
	dynamic_cast<RenderWindow*>(this->components["RENDER"])->setTexture(texture);
}

UI::~UI() {
	for (auto &component : components) {
		delete component.second;
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
