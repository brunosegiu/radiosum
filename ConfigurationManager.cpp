#include "ConfigurationManager.h"

#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>

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


// From: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string& s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

ConfigurationManager::ConfigurationManager() {
	std::ifstream input("config.ini");
	std::string line;
	while (getline(input, line)) {
		if (line.size() > 0 && line[0] != '#') {
			std::vector<std::string> parsed = split(line, '=');
			this->configuration[parsed[0]] = parsed[1];
		}
	}
}


ConfigurationManager::~ConfigurationManager() {
}
