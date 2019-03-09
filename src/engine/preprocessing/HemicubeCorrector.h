#pragma once

#include "Texture.h"

class HemicubeCorrector {
public:
	HemicubeCorrector(GLuint width);
	void read();
	virtual ~HemicubeCorrector();
private:
	Texture *top, *side;
};

