#pragma once

#include "UIntBuffer.h"
#include "Camera.h"

enum Face { FRONT, RIGHT, LEFT, TOP, BOTTOM };

class Hemicube : public Buffer {
public:
	Hemicube(GLuint width);
	void bind(Face face);
	void clean();
	void read();
	Camera* getCamera(Camera* frontFacingCam);
	virtual ~Hemicube();
private:
	UIntBuffer *front, *half;
	Face boundFace;
	GLuint width;
	Camera* internalCam;
};

