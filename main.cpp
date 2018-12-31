#include <windows.h>

#include "Window.h"

// Enable Nvidia Graphics Card use by deafault
extern "C" {
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main(int argc, char *args[]) {
  Window *win = new Window(800, 600, "Radiosum");
  while (true) win->update();
  delete win;
  return 0;
}