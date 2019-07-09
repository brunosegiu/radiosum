#pragma once

#include "EngineTypes.h"

#include "Camera.h"
#include "geometry/Scene.h"

#include "Renderer.h"

#include "display/SceneRenderer.h"
#include "preprocessing/PreprocessingController.h"

#include "display/DisplayController.h"

class Engine {
 public:
  Engine();

  // Internal state
  void setMode(RenderMode mode);
  RenderMode getMode();
  void step();

  // Lighting
  void preprocess(bool withOutput = true, Reflectors reflector = DISABLED,
                  RendererType renderer = OPENGL);
  void computeRadiosity(std::vector<Channel> channels,
                        SolverType solver = SLU, bool smooth = false);
  void setRadiosity(bool smooth);
  Camera* getCamera();

  // Geometry
  Scene* getScene();
  void resetScene();

  GLuint pick(GLint x, GLint y);

  // Storing

  void exportGeometry(std::string path);
  void exportFFMatrix(std::string path);
  void exportEmission(std::string path);
  void exportReflactance(std::string path);
  void exportRadiosity(std::string path);

  void importGeometry(std::string path);
  void importFFMatrix(std::string path);
  void importEmission(std::string path);
  void importReflactance(std::string path);
  void importRadiosity(std::string path, bool smooth);

  virtual ~Engine();

 public:
  GLuint outputTexture;

 private:
  Camera* camera;
  Scene* scene;

  RenderMode mode;
  SceneRenderer* sceneRenderer;
  PreprocessingController* preprocessor;
  DisplayController* displayer;
};
