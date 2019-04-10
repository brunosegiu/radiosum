#include "components/RenderWindow.h"

#include <string>

#include <imgui.h>

#include "ConfigurationManager.h"
#include "UIStore.h"

RenderWindow::RenderWindow() : Component() {}

void RenderWindow::setTexture(GLuint texture) {
  this->enabled = true;
  this->texture = texture;
}

void RenderWindow::render() {
  if (this->enabled) {
    this->update();
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
}

void RenderWindow::update() {
  mousePos = ImGui::GetMousePos();
  mousePos = ImVec2((1.0f / GLfloat(this->scale)) *
                        (mousePos.x - (this->appWidth - this->width) / 2.0f),
                    (1.0f / GLfloat(this->scale)) * (mousePos.y - 20.0f));
  if (ImGui::IsMouseDoubleClicked(0)) {
    GLuint picked = UIStore::engine->pick(mousePos.x, mousePos.y);
    if (picked > 0) {
      UIStore::selectedFace = GLint(picked) - 1;
    }
  } else if (ImGui::IsMouseDoubleClicked(1)) {
    GLuint picked = UIStore::engine->pick(mousePos.x, mousePos.y);
    if (picked > 0 && picked < UIStore::engine->getScene()->size() + 1) {
      Face face = UIStore::engine->getScene()->getFace(picked - 1);
      UIStore::camera->goTo(face);
    }
  }
}

RenderWindow::~RenderWindow() {}
