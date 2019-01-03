#pragma once

#include <vector>

#include "IDRenderer.h"
#include "BufferRenderer.h"
#include "Mesh.h"
#include "UIntBuffer.h"

class PreprocessingController {
public:
	PreprocessingController(std::vector<Mesh*> meshes);
	void reset();
	void runStep();
	bool end();
	virtual ~PreprocessingController();
private:
	IDRenderer* renderer;
	BufferRenderer* bufferRenderer;
	std::vector<Mesh*> meshes;
	unsigned int meshStep, faceStep;
	Camera* cam;
	UIntBuffer* buffer;
};

