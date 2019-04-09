#pragma once

#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>

#define LOG_MAX_BUFFER_S 200
#define VAR_MAX_BUFFER_S 100

class Logger {
 public:
  Logger();
  ~Logger();
  void log(std::string name, GLfloat value);
  void log(std::string name, GLint value);
  void log(std::string line);
  void clean();
  std::vector<std::string> getLog();
  std::map<std::string, std::vector<GLfloat>> getFloats();
  std::map<std::string, std::vector<GLint>> getInts();

 private:
  std::map<std::string, std::vector<GLfloat>> floats;
  std::map<std::string, std::vector<GLint>> ints;
  std::vector<std::string> logs;
};
