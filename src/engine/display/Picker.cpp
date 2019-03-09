#include "display/Picker.h"

Picker::Picker(GLuint maxSize, PickingBuffer* buffer) {
	this->maxSize = maxSize;
	this->buffer = buffer;
}


GLuint Picker::getSelected(GLint x, GLint y) {
	if (x >= 0 && y >= 0) {
		if (this->buffer) {
			GLuint candidate = this->buffer->getSelectedItem(x, y);
			if (candidate <= this->maxSize) {
				return candidate;
			}
		}

	}
	return 0;
}

Picker::~Picker() {
}
