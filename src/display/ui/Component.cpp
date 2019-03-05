#include "Component.h"



Component::Component() {
	this->enabled = false;
}

void Component::enable() {
	this->enabled = true;
}


Component::~Component() {
}
