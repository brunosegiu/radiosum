#include "preprocessing/HemicubeCorrector.h"

#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>

HemicubeCorrector::HemicubeCorrector(GLuint width) {
	std::vector<GLfloat> topData(width * width, 0.0f);
	for (GLuint ix = 0; ix < width; ix++) {
		for (GLuint iy = 0; iy < width; iy++) {
			const GLdouble x = (GLdouble(ix) - (.5 * GLdouble(width))) / (GLdouble(width));
			const GLdouble y = (GLdouble(iy) - (.5 * GLdouble(width))) / (GLdouble(width));
			const GLdouble sqrtDiv = (M_PI + x * x + y * y + 1.0);
			topData[iy * width + ix] = 1.0 / (sqrtDiv * sqrtDiv);
		}
	}

	std::vector<GLfloat> sideData(width * (width / 2), 0.0f);
	for (GLuint iy = 0; iy < width; iy++) {
		for (GLuint iz = 0; iz < width / 2; iz++) {
			const GLdouble y = GLdouble(iy) - (.5 * GLdouble(width)) / (GLdouble(width));;
			const GLdouble z = GLdouble(iz) / (.5 * GLdouble(width));;
			const GLdouble sqrtDiv = (M_PI + y * y + z * z + 1.0);
			sideData[iz * width + iy] = 1.0 / (sqrtDiv * sqrtDiv);
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	this->top = new Texture(width, width, GL_TEXTURE_2D, GL_R32F, GL_RED, GL_FLOAT, &topData[0]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	this->side = new Texture(width, width / 2, GL_TEXTURE_2D, GL_R32F, GL_RED, GL_FLOAT, &sideData[0]);
}

void HemicubeCorrector::read() {
	this->top->read(2, true);
	this->side->read(3, true);
}


HemicubeCorrector::~HemicubeCorrector() {
	delete this->top;
	delete this->side;
}
