#include "components/RenderWindow.h"

#include <string>

#include <imgui.h>

#include "ConfigurationManager.h"

RenderWindow::RenderWindow() : Component() {}

void RenderWindow::setTexture(GLuint texture) {
  this->enabled = true;
  this->texture = texture;
}

void RenderWindow::render() {
  ImGui::SetNextWindowSize(ImVec2(this->width, this->height));
  ImGui::SetNextWindowPos(ImVec2((this->appWidth - this->width) / 2, 20.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  if (this->enabled &&
      ImGui::Begin(
          "Render", nullptr,
          ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs |
              ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize |
              ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
              ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration)) {
    ImGui::Image((void*)this->texture, ImVec2(this->width, this->height),
                 ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
  }
  ImGui::PopStyleVar();
}

RenderWindow::~RenderWindow() {}
