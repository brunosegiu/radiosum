#include "Logger.h"

Logger::Logger() {
}

void Logger::log(std::string name, GLfloat value) {
	if (this->floats.count(name) == 0)
		this->floats[name] = std::vector<GLfloat>();
	else if (this->floats[name].size() >= VAR_MAX_BUFFER_S)
		this->floats[name].erase(this->floats[name].begin());
	this->floats[name].push_back(value);
}

void Logger::log(std::string name, GLint value) {
	if (this->ints.count(name) == 0)
		this->ints[name] = std::vector<GLint>();
	else if (this->ints[name].size() >= VAR_MAX_BUFFER_S)
		this->ints[name].erase(this->ints[name].begin());
	this->ints[name].push_back(value);
}

void Logger::log(std::string line) {
	if (this->logs.size() == LOG_MAX_BUFFER_S)
		this->logs.erase(this->logs.begin());
	this->logs.push_back(line);
}

void Logger::clean() {
	this->logs.clear();
	this->floats.clear();
	this->ints.clear();
}

std::vector<std::string> Logger::getLog() {
	return this->logs;
}

std::map<std::string, std::vector<GLfloat>> Logger::getFloats() {
	return this->floats;
}

std::map<std::string, std::vector<GLint>> Logger::getInts() {
	return this->ints;
}

Logger::~Logger() {
}
