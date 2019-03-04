#include "common/Timer.h"



Timer::Timer() {
	this->start = std::clock();
}

float Timer::get() {
	return (std::clock() - start) / (float)CLOCKS_PER_SEC;
}


Timer::~Timer() {
}
