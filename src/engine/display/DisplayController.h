#pragma once

#include "Camera.h"
#include "EngineTypes.h"
#include "display/SceneRenderer.h"
#include "display/TextureRenderer.h"

class DisplayController {
 public:
  DisplayController(RenderMode mode);
  void render();
  GLuint read();
  void setMode(RenderMode mode);
  virtual ~DisplayController();

 private:
  Renderer* renderer;
  TextureRenderer* bufferRenderer;
  TextureRenderer* radiosityRenderer;
  TextureRenderer* emissionsRenderer;
  TextureRenderer* facesRenderer;
  TextureRenderer* reflactancesRenderer;
  TextureRenderer* albedoRenderer;

  Camera* userCamera;

  RenderMode mode;
};
