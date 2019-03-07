#pragma once

#include <string>
#include <map>
#include <vector>

#include <GL/glew.h>

#define LOG_MAX_BUFFER_S 20
#define VAR_MAX_BUFFER_S 100

class Logger {
public:
	static void log(std::string name, GLfloat value);
	static void log(std::string name, GLint value);
	static void log(std::string line);
	static void clean();
	static std::vector<std::string> getLog();
	static std::map<std::string, std::vector<GLfloat>> getFloats();
	static std::map<std::string, std::vector<GLint>> getInts();
private:
	std::map<std::string, std::vector<GLfloat>> floats;
	std::map<std::string, std::vector<GLint>> ints;
	std::vector<std::string> logs;
	static Logger* instance;
	Logger();
	~Logger();
	static Logger* getInstance();
};

