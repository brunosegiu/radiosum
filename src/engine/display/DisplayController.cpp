#include "display/DisplayController.h"

DisplayController::DisplayController(RenderMode mode) {
  this->bufferRenderer = new TextureRenderer("renderHemicube.frag");
  this->radiosityRenderer = new TextureRenderer("renderSceneRadiosity.frag");
  this->emissionsRenderer = new TextureRenderer("renderSceneEmissions.frag");
  this->facesRenderer = new TextureRenderer("renderSceneFaces.frag");
  this->albedoRenderer = new TextureRenderer("renderSceneAlbedo.frag");
  this->reflactancesRenderer =
      new TextureRenderer("renderSceneReflactances.frag");

  this->userCamera = new Camera();
  this->radiosityRenderer->setCamera(userCamera);
  this->emissionsRenderer->setCamera(userCamera);
  this->facesRenderer->setCamera(userCamera);

  this->renderer = this->radiosityRenderer;
  this->mode = mode;
  this->setMode(mode);
}

void DisplayController::render() { this->renderer->render(); }

GLuint DisplayController::read() { return this->renderer->read(); }

void DisplayController::setMode(RenderMode mode) {
  this->mode = mode;
  switch (mode) {
    case PREPROCESS:
      this->renderer = this->bufferRenderer;
      break;
    case RADIOSITY:
      this->renderer = this->radiosityRenderer;
      break;
    case EMISSION:
      this->renderer = this->emissionsRenderer;
      break;
    case FACES:
      this->renderer = this->facesRenderer;
      break;
    case REFLACTANCE:
      this->renderer = this->reflactancesRenderer;
      break;
    case ALBEDO:
      this->renderer = this->albedoRenderer;
      break;
  }
}

DisplayController::~DisplayController() { delete this->bufferRenderer; }
