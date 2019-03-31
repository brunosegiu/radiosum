#include "Component.h"

#include "ConfigurationManager.h"

Component::Component() {
  this->enabled = false;
  this->width = std::stoi(ConfigurationManager::get("APP_WINDOW_WIDTH"));
  this->height = std::stoi(ConfigurationManager::get("APP_WINDOW_HEIGHT"));
  this->appWidth = std::stoi(ConfigurationManager::get("APP_RES_WIDTH"));
  this->appHeight = std::stoi(ConfigurationManager::get("APP_RES_HEIGHT"));
  this->scale = std::stof(ConfigurationManager::get("APP_SCALE"));
  this->width = this->width * scale;
  this->height = this->height * scale;
}

void Component::enable() { this->enabled = true; }

Component::~Component() {}
