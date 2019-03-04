#include "common/Logger.h"

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance() {
	if (Logger::instance == nullptr)
		Logger::instance = new Logger();
	return Logger::instance;
}

void Logger::log(std::string name, std::string value) {
	Logger* inst = Logger::getInstance();
	inst->vars[name] = value;
}

void Logger::log(std::string line) {
	Logger* inst = Logger::getInstance();
	if (inst->logs.size() == LOG_MAX_BUFFER_S)
		inst->logs.erase(inst->logs.begin());
	inst->logs.push_back(line);
}

void Logger::clean() {
	Logger* inst = Logger::getInstance();
	inst->logs.clear();
	inst->vars.clear();
}

std::vector<std::string> Logger::getLog() {
	Logger* inst = Logger::getInstance();
	return inst->logs;
}

std::map<std::string, std::string> Logger::getVars() {
	Logger* inst = Logger::getInstance();
	return inst->vars;
}

Logger::Logger() {
}


Logger::~Logger() {
}
