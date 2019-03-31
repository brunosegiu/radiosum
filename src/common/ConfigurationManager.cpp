#include "ConfigurationManager.h"

#include <fstream>
#include <vector>

#include "Utils.h"

ConfigurationManager* ConfigurationManager::config = nullptr;

ConfigurationManager* ConfigurationManager::getInstance() {
  if (ConfigurationManager::config == nullptr) {
    ConfigurationManager::config = new ConfigurationManager();
  }
  return ConfigurationManager::config;
}

std::string ConfigurationManager::get(std::string id) {
  ConfigurationManager* manager = ConfigurationManager::getInstance();
  return manager->configuration[id];
}

ConfigurationManager::ConfigurationManager() {
  std::ifstream input("config.ini");
  std::string line;
  while (getline(input, line)) {
    if (line.size() > 0 && line[0] != '#') {
      std::vector<std::string> parsed = split(line, "=");
      this->configuration[parsed[0]] = parsed[1];
    }
  }
}

ConfigurationManager::~ConfigurationManager() {}
