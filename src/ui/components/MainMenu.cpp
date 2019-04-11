#include "components/MainMenu.h"

#include <string>

#include "EngineStore.h"
#include "UIStore.h"
#include "Utils.h"

MainMenu::MainMenu() {}

void MainMenu::render() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New")) {
        UIStore::engine->resetScene();
      }
      if (ImGui::BeginMenu("Import")) {
        if (ImGui::MenuItem("Geometry")) {
          std::string path = selectFile();
          if (path != "") UIStore::engine->importGeometry(path);
        }
        if (ImGui::MenuItem("Emission", nullptr, false,
                            UIStore::engine->getScene()->size() > 0)) {
          std::string path = selectFile();
          if (path != "") UIStore::engine->importEmission(path);
        }
        if (ImGui::MenuItem("Reflactance", nullptr, false,
                            UIStore::engine->getScene()->size() > 0)) {
          std::string path = selectFile();
          if (path != "") UIStore::engine->importReflactance(path);
        }
        if (ImGui::MenuItem("Form factor matrix", nullptr, false,
                            UIStore::engine->getScene()->size() > 0)) {
          std::string path = selectFile(true);
          if (path != "") UIStore::engine->importFFMatrix(path);
        }
        if (ImGui::MenuItem("Radiosity", nullptr, false,
                            UIStore::engine->getScene()->size() > 0)) {
          std::string path = selectFile();
          if (path != "")
            UIStore::engine->importRadiosity(path, UIStore::shading == GOURAUD);
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Export")) {
        if (ImGui::MenuItem("Geometry")) {
          std::string path = selectFile(true);
          if (path != "") UIStore::engine->exportRadiosity(path);
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Outputs a text file, format is obj");
          ImGui::EndTooltip();
        }
        if (ImGui::MenuItem("Emission")) {
          std::string path = selectFile(true);
          if (path != "") UIStore::engine->exportEmission(path);
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Outputs a text file, format is: Row Emission eol");
          ImGui::EndTooltip();
        }
        if (ImGui::MenuItem("Reflactance")) {
          std::string path = selectFile(true);
          if (path != "") UIStore::engine->exportReflactance(path);
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Outputs a text file, format is: Row RefR RefG RefB eol");
          ImGui::EndTooltip();
        }
        if (ImGui::MenuItem("Form factor matrix")) {
          std::string path = selectFile(true);
          if (path != "") UIStore::engine->exportFFMatrix(path);
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Outputs a text file, format is: Row Col FF eol");
          ImGui::EndTooltip();
        }
        if (ImGui::MenuItem("Radiosity")) {
          std::string path = selectFile(true);
          if (path != "") UIStore::engine->exportRadiosity(path);
        }
        if (ImGui::IsItemHovered()) {
          ImGui::BeginTooltip();
          ImGui::Text("Outputs a text file, format is: Row RadR RadG RadB eol");
          ImGui::EndTooltip();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings")) {
      if (ImGui::BeginMenu("Render")) {
        if (ImGui::BeginMenu("Radiosity")) {
          bool enabled = UIStore::engine->getMode() == RADIOSITY;
          if (ImGui::MenuItem("Flat", nullptr,
                              enabled && UIStore::shading == FLAT)) {
            UIStore::engine->setMode(RADIOSITY);
            if (EngineStore::pipelineStage == RADIOSITY_READY) {
              UIStore::engine->setRadiosity(false);
              UIStore::shading = FLAT;
            }
          }
          if (ImGui::MenuItem("Gouraud", nullptr,
                              enabled && UIStore::shading == GOURAUD)) {
            UIStore::engine->setMode(RADIOSITY);
            if (EngineStore::pipelineStage == RADIOSITY_READY) {
              UIStore::engine->setRadiosity(true);
              UIStore::shading = GOURAUD;
            }
          }
          ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Emission", nullptr,
                            UIStore::engine->getMode() == EMISSION)) {
          UIStore::engine->setMode(EMISSION);
        }
        if (ImGui::MenuItem("Reflactance", nullptr,
                            UIStore::engine->getMode() == REFLACTANCE)) {
          UIStore::engine->setMode(REFLACTANCE);
        }

        if (ImGui::MenuItem("Faces", nullptr,
                            UIStore::engine->getMode() == FACES)) {
          UIStore::engine->setMode(FACES);
        }

        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Debug")) {
        if (ImGui::MenuItem("Render preprocess pass", nullptr,
                            UIStore::enablePreprocessRendering)) {
          UIStore::enablePreprocessRendering =
              !UIStore::enablePreprocessRendering;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

MainMenu::~MainMenu() {}