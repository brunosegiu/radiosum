#include "preprocessing/HemicubeCorrector.h"

#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>

HemicubeCorrector::HemicubeCorrector(GLuint width) {
	std::vector<GLfloat> topData(width * width, 0.0f);
	const double halfWidth = (.5 * GLdouble(width));
	const double normalizeTerm = 1.0 / GLdouble(width * width * 3);
	for (GLuint ix = 0; ix < width; ix++) {
		for (GLuint iy = 0; iy < width; iy++) {
			GLdouble x = (GLdouble(ix) - halfWidth) / halfWidth;
			GLdouble y = (GLdouble(iy) - halfWidth) / halfWidth;
			GLdouble sqrtDiv = (x * x + y * y + 1.0);
			topData[ix * width + iy] = normalizeTerm / (M_PI*(sqrtDiv * sqrtDiv));
		}
	}

	std::vector<GLfloat> sideData(width * (width / 2), 0.0f);
	for (GLuint iy = 0; iy < width; iy++) {
		for (GLuint iz = 0; iz < width / 2; iz++) {
			GLdouble y = (GLdouble(iy) - halfWidth) / halfWidth;
			GLdouble z = GLdouble(iz) / halfWidth;
			GLdouble sqrtDiv = (y * y + z * z + 1.0);
			sideData[iy * width / 2 + iz] = normalizeTerm * (z / (M_PI*(sqrtDiv * sqrtDiv)));
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
