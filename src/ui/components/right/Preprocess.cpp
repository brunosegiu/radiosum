#include "components/right/Preprocess.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "EngineStore.h"
#include "UIStore.h"

Preprocess::Preprocess() : Component() {
  this->enable();
  this->smooth = false;
  this->channelCount = 0;
}

void Preprocess::render() {
  if (enabled && ImGui::BeginTabItem("Preprocess")) {
    ImGui::TextColored(ImVec4(.0f, 1.0f, .0f, 1.0f), "Step 1: ");
    ImGui::SameLine();
    ImGui::Text("Compute Form Factors");

    float progress = EngineStore::ffProgress;
    float radProgress = EngineStore::radiosityProgress;
    ImGui::Spacing();
    ImGui::Text("Progress");
    ImGui::ProgressBar(progress);
    if (ImGui::Button("Compute form factors")) {
      UIStore::engine->preprocess(UIStore::enablePreprocessRendering);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(.0f, 1.0f, .0f, 1.0f), "Step 2: ");
    ImGui::SameLine();
    ImGui::Text("Compute radiosity");

    bool enableStep2 = EngineStore::pipelineStage == FF_READY ||
                       EngineStore::pipelineStage == RADIOSITY_READY;
    if (!enableStep2) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    if (ImGui::RadioButton("Interpolate", this->smooth)) {
      if (!this->smooth && EngineStore::pipelineStage == RADIOSITY_READY) {
        UIStore::engine->setRadiosity(!smooth);
      }
      this->smooth = true;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Flatten", !this->smooth)) {
      if (this->smooth && EngineStore::pipelineStage == RADIOSITY_READY) {
        UIStore::engine->setRadiosity(!smooth);
      }
      this->smooth = false;
    }
    ImGui::Combo("Channels", &this->channelCount, "Single\0Double\0Triple");
    ImGui::Combo("Solver", &this->channelCount,
                 "Eigen: SparseLU\0Eigen: Cholesky\0Other");
    ImGui::Text("Progress");
    ImGui::ProgressBar(radProgress);
    if (ImGui::Button("Compute radiosity")) {
      std::vector<Channel> channels = {RED, GREEN, BLUE};
      UIStore::engine->computeRadiosity(
          std::vector<Channel>(channels.begin(),
                               channels.begin() + this->channelCount + 1),
          this->smooth);
    }
    if (!enableStep2) {
      ImGui::PopItemFlag();
      ImGui::PopStyleVar();
    }
    ImGui::EndTabItem();
  }
}

Preprocess::~Preprocess() {}
