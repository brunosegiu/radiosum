#include "preprocessing/opengl/HemicubeCorrector.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "EngineStore.h"

HemicubeCorrector::HemicubeCorrector(GLuint width) {
  std::vector<GLfloat> topData(width * width, 0.0);
  const GLint halfWidth = GLdouble(width) / 2;
  const double deltaA = 4.0 / (GLdouble(width) * GLdouble(width));

  for (GLint ix = 0; ix < width; ix++) {
    for (GLint iy = 0; iy < width; iy++) {
      GLdouble x = 2.0 * (ix - halfWidth + .5) / GLdouble(width);
      GLdouble y = 2.0 * (iy - halfWidth + .5) / GLdouble(width);
      GLdouble sqrtDiv = (x * x + y * y + 1.0);
      GLdouble term = (1.0 / (sqrtDiv * sqrtDiv)) * (1.0 / M_PI) * deltaA;
      topData[ix * width + iy] = term;
    }
  }

  std::vector<GLfloat> sideData(width * halfWidth, 0.0);
  for (GLint iy = 0; iy < width; iy++) {
    for (GLint iz = 0; iz < halfWidth; iz++) {
      GLdouble y = 2.0 * (iy - halfWidth + .5) / GLdouble(width);
      GLdouble z = 2.0 * (iz + .5) / GLdouble(width);
      GLdouble sqrtDiv = (y * y + z * z + 1.0);
      GLdouble term = (z / (sqrtDiv * sqrtDiv)) * (1.0 / M_PI) * deltaA;
      sideData[iy * halfWidth + iz] = term;
    }
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  this->top = new Texture(width, width, GL_TEXTURE_2D, GL_R32F, GL_RED,
                          GL_FLOAT, &topData[0]);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  this->side = new Texture(width, halfWidth, GL_TEXTURE_2D, GL_R32F, GL_RED,
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
