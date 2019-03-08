#pragma once

#include "common/Texture.h"

class HemicubeCorrector {
public:
	HemicubeCorrector(GLuint width);
	void read();
	virtual ~HemicubeCorrector();
private:
	Texture *top, *side;
};

