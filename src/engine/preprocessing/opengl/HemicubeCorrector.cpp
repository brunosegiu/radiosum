#include "preprocessing/opengl/HemicubeCorrector.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "EngineStore.h"

HemicubeCorrector::HemicubeCorrector(GLuint width) {
  std::vector<GLfloat> topData(width * width, 0.0);
  const GLuint halfWidth = GLdouble(width) / 2;
  const double deltaA = 1.0 / GLdouble(width * width);

  GLdouble acum = 0;
  GLdouble center = 0;
  for (GLuint ix = 0; ix < width; ix++) {
    for (GLuint iy = 0; iy < width; iy++) {
      GLdouble x = ((ix - halfWidth) / GLdouble(halfWidth));
      GLdouble y = ((iy - halfWidth) / GLdouble(halfWidth));
      GLdouble sqrtDiv = (x * x + y * y + 1.0);
      GLdouble term = 1.0 / (M_PI * (sqrtDiv * sqrtDiv));
      term = term * deltaA;
      acum += term;
      topData[ix * width + iy] = term;
    }
  }

  std::vector<GLfloat> sideData(width * halfWidth, 0.0);
  for (GLuint iy = 0; iy < width; iy++) {
    for (GLuint iz = 0; iz < halfWidth; iz++) {
      GLdouble y = ((iy - halfWidth) / GLdouble(halfWidth));
      GLdouble z = (iz / GLdouble(halfWidth));
      GLdouble sqrtDiv = (y * y + z * z + 1.0);
      GLdouble term = (z / (M_PI * (sqrtDiv * sqrtDiv)));
      term = term * deltaA;
      acum += 4.0 * term;
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
