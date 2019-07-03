#include "UIStore.h"

Engine* UIStore::engine = nullptr;
bool UIStore::selectFace = false;
bool UIStore::enableSettings = true;
int UIStore::selectedFace = -1;
int UIStore::reflections = 0;
bool UIStore::enablePreprocessRendering = true;
ShadingModel UIStore::shading = FLAT;
CameraController* UIStore::camera = nullptr;