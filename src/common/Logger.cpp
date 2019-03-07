#include "common/Logger.h"

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance() {
	if (Logger::instance == nullptr)
		Logger::instance = new Logger();
	return Logger::instance;
}

void Logger::log(std::string name, GLfloat value) {
	Logger* inst = Logger::getInstance();
	if (inst->floats.count(name) == 0)
		inst->floats[name] = std::vector<GLfloat>();
	else if (inst->floats[name].size() >= VAR_MAX_BUFFER_S)
		inst->floats[name].erase(inst->floats[name].begin());
	inst->floats[name].push_back(value);
}

void Logger::log(std::string name, GLint value) {
	Logger* inst = Logger::getInstance();
	if (inst->ints.count(name) == 0)
		inst->ints[name] = std::vector<GLint>();
	else if (inst->ints[name].size() >= VAR_MAX_BUFFER_S)
		inst->ints[name].erase(inst->ints[name].begin());
	inst->ints[name].push_back(value);
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
	inst->floats.clear();
	inst->ints.clear();
}

std::vector<std::string> Logger::getLog() {
	Logger* inst = Logger::getInstance();
	return inst->logs;
}

std::map<std::string, std::vector<GLfloat>> Logger::getFloats() {
	Logger* inst = Logger::getInstance();
	return inst->floats;
}

std::map<std::string, std::vector<GLint>> Logger::getInts() {
	Logger* inst = Logger::getInstance();
	return inst->ints;
}

Logger::Logger() {
}


Logger::~Logger() {
}
