#pragma once

#include <SDL.h>

class EventListener {
 public:
  virtual void process(SDL_Event &event) = 0;
  inline virtual ~EventListener() {}
};
