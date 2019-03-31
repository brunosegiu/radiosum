#pragma once

#include <map>
#include <string>

class ConfigurationManager {
 public:
  static std::string get(std::string id);

 private:
  std::map<std::string, std::string> configuration;
  static ConfigurationManager* getInstance();
  static ConfigurationManager* config;
  ConfigurationManager();
  virtual ~ConfigurationManager();
};
