#include "UIStore.h"

Engine* UIStore::engine = nullptr;
bool UIStore::selectFace = false;
int UIStore::selectedFace = -1;
bool UIStore::enablePreprocessRendering = true;
ShadingModel UIStore::shading = FLAT;
CameraController* UIStore::camera = nullptr;