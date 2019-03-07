#include "Component.h"

#include "common/ConfigurationManager.h"

Component::Component() {
	this->enabled = false;
	this->width = std::stoi(ConfigurationManager::get("APP_WINDOW_WIDTH"));
	this->height = std::stoi(ConfigurationManager::get("APP_WINDOW_HEIGHT"));
	this->appWidth = std::stoi(ConfigurationManager::get("APP_RES_WIDTH"));
	this->appHeight = std::stoi(ConfigurationManager::get("APP_RES_HEIGHT"));
}

void Component::enable() {
	this->enabled = true;
}


Component::~Component() {
}
