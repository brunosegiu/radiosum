#include "display/ui/UI.h"

#include "display/ui/DebugInfo.h"
#include "display/ui/Menu.h"
#include "common/Logger.h"

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

}

void UI::process(SDL_Event &event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void UI::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	renderMenu();
	renderDebugInfo();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


UI::~UI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
