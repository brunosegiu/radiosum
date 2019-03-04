#pragma once

#include <string>
#include <map>
#include <vector>

#define LOG_MAX_BUFFER_S 20

class Logger {
public:
	static void log(std::string name, std::string value);
	static void log(std::string line);
	static void clean();
	static std::vector<std::string> getLog();
	static std::map<std::string, std::string> getVars();
private:
	std::map<std::string, std::string> vars;
	std::vector<std::string> logs;
	static Logger* instance;
	Logger();
	~Logger();
	static Logger* getInstance();
};

