#include "../include/XManager.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>

Displayable::Displayable(std::shared_ptr<DisplayVisitable> dv) {
  displayable = dv;
}

XManager::XManager(int windowWidth, int windowHeight, int borderWidth) {
  Display *display = XOpenDisplay(NULL);
  if (display == NULL) {
    std::cout << "Could not open display." << std::endl;
    exit(EXIT_FAILURE);
  }

  int screenNum = DefaultScreen(display);
  Window window = XCreateSimpleWindow(
      display, RootWindow(display, screenNum), WINDOW_DEFAULT_X, WINDOW_DEFAULT_Y, windowWidth,
      windowHeight, borderWidth, BlackPixel(display, screenNum),
      BlackPixel(display, screenNum));

  XSelectInput(display, window,
               KeyPressMask | KeyReleaseMask | StructureNotifyMask);
  XMapWindow(display, window);

  GC gc = XCreateGC(display, window, 0, nullptr);
}
