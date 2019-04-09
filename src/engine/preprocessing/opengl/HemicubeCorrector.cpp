#include "preprocessing/opengl/HemicubeCorrector.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "EngineStore.h"

HemicubeCorrector::HemicubeCorrector(GLuint width) {
  std::vector<GLfloat> topData(width * width, 0.0f);
  const double halfWidth = (.5 * GLdouble(width));
  const double normalizationFactor = 1.0 / GLdouble(width * width * 3);
  for (GLuint ix = 0; ix < width; ix++) {
    for (GLuint iy = 0; iy < width; iy++) {
      GLdouble x = 2.0 * ((GLdouble(ix) / GLdouble(width)) - .5);
      GLdouble y = 2.0 * ((GLdouble(iy) / GLdouble(width)) - .5);
      GLdouble sqrtDiv = (x * x + y * y + 1.0);
      GLfloat term = 1.0 / (M_PI * (sqrtDiv * sqrtDiv));
      topData[ix * width + iy] = normalizationFactor * term;
    }
  }

  std::vector<GLfloat> sideData(width * (width / 2), 0.0f);
  for (GLuint iy = 0; iy < width; iy++) {
    for (GLuint iz = 0; iz < GLuint(halfWidth); iz++) {
      GLdouble y = 2.0 * ((GLdouble(iy) / GLdouble(width)) - .5);
      GLdouble z = 2.0 * ((GLdouble(iz) / GLdouble(width)) - .5);
      GLdouble sqrtDiv = (y * y + z * z + 1.0);
      GLfloat term = (abs(z) / (M_PI * (sqrtDiv * sqrtDiv)));
      sideData[iy * halfWidth + iz] = normalizationFactor * term;
    }
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  this->top = new Texture(width, width, GL_TEXTURE_2D, GL_R32F, GL_RED,
                          GL_FLOAT, &topData[0]);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  this->side = new Texture(width, width / 2, GL_TEXTURE_2D, GL_R32F, GL_RED,
                           GL_FLOAT, &sideData[0]);
}

void HemicubeCorrector::read() {
  this->top->read(2, true);
  this->side->read(3, true);
}

HemicubeCorrector::~HemicubeCorrector() {
  delete this->top;
  delete this->side;
}
