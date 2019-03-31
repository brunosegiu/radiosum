#pragma once

enum RenderMode { RADIOSITY, EMISSION, FACES, REFLACTANCE, PREPROCESS };

#define N_CHANNELS 3

enum Channel { RED, GREEN, BLUE };

enum PipelineStage {
  INIT,
  FF_LOADING,
  FF_READY,
  RADIOSITY_LOADING,
  RADIOSITY_READY
};