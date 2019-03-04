#pragma once

#include <ctime>

class Timer {
public:
	Timer();
	float get();
	virtual ~Timer();
private:
	std::clock_t start;
};

