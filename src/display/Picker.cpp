#include "display/Picker.h"

#include "common/Application.h"

Picker::Picker() {
}


GLuint Picker::getSelected(GLint x, GLint y) {
	if (x >= 0 && y >= 0) {
		PickingBuffer* buffer = Application::getPickingBuffer();
		if (buffer) {
			GLuint candidate = buffer->getSelectedItem(x, y);
			if (candidate <= Application::getScene()->size()) {
				return candidate;
			}
		}

	}
	return 0;
}

Picker::~Picker() {
}
