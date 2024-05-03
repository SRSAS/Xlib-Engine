#include "../include/XManager.h"
#include <X11/keysym.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>

Displayable::Displayable(std::shared_ptr<DisplayVisitable> &dv) {
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
      display, RootWindow(display, screenNum), WINDOW_DEFAULT_X,
      WINDOW_DEFAULT_Y, windowWidth, windowHeight, borderWidth,
      BlackPixel(display, screenNum), BlackPixel(display, screenNum));

  XSelectInput(display, window,
               KeyPressMask | KeyReleaseMask | StructureNotifyMask);
  XMapWindow(display, window);

  GC gc = XCreateGC(display, window, 0, nullptr);
}

void XManager::visitRectangle(const Rectangle &rectangle) {
  XFillRectangle(display, window, gc, (int)rectangle.position.x,
                 (int)rectangle.position.y, rectangle.width, rectangle.height);
}

void XManager::updateWindowSize() {
  XWindowAttributes attributes;
  XGetWindowAttributes(display, window, &attributes);
  windowWidth = attributes.width;
  windowHeight = attributes.height;
}

Key XManager::convertXKtoKey(int xk_key) {
  switch (xk_key) {
  case XK_Q:
    return KEY_SPACE;
  case XK_space:
    return KEY_SPACE;
  case XK_Left:
    return KEY_LEFT;
  case XK_Right:
    return KEY_RIGHT;
  default:
    return NO_KEY;
  }
}

void XManager::removeKeyFromKeysPressed(Key key) {
  auto iter = keysPressed.begin();
  while (iter != keysPressed.end() && (*iter) != key) {
    iter++;
  }
  keysPressed.erase(iter);
}

void XManager::addObserver(std::shared_ptr<Observer> observer) {
  observers.push_back(observer);
}

void XManager::removeObserver(std::shared_ptr<Observer> &observer) {
  auto iter = observers.begin();
  while (iter != observers.end() && *iter != observer) {
    iter++;
  }
  if (iter == observers.end()) {
    return;
  }
  observers.erase(iter);
}

void XManager::notifyAll() {
  for (auto iter = observers.begin(); iter != observers.end(); iter++) {
    (*iter)->onNotified();
  }
}

void XManager::onNotified() {
  erase();
  draw();
}

void XManager::addDisplayable(std::shared_ptr<DisplayVisitable> object) {
  auto displayable = std::make_unique<Displayable>(object);
  displayables.push_back(displayable);
}

bool XManager::removeDisplayable(
    std::shared_ptr<DisplayVisitable> &displayable) {
  auto iter = displayables.begin();
  while (iter != displayables.end() && (*iter)->displayable != displayable) {
    iter++;
  }

  if (iter == displayables.end()) {
    return false;
  }

  displayables.erase(iter);
  return true;
}

void XManager::setInvisible(std::shared_ptr<DisplayVisitable> &displayable) {
  auto iter = displayables.begin();
  while (iter != displayables.end() && (*iter)->displayable != displayable) {
    iter++;
  }

  if (iter == displayables.end()) {
    return;
  }

  (*iter)->display = false;
}
void XManager::setVisible(std::shared_ptr<DisplayVisitable> &displayable) {
  auto iter = displayables.begin();
  while (iter != displayables.end() && (*iter)->displayable != displayable) {
    iter++;
  }

  if (iter == displayables.end()) {
    return;
  }

  (*iter)->display = true;
}

void XManager::draw() {
  for (auto iter = displayables.begin(); iter != displayables.end(); iter++) {
    (*iter)->displayable->accept(*this);
  }
}

void XManager::erase() {
  XSetForeground(display, gc, BlackPixel(display, screenNum));
  XFillRectangle(display, window, gc, 0, 0, windowWidth, windowHeight);
  XSetForeground(display, gc, WhitePixel(display, screenNum));
}

void XManager::handleEvents() {
  XEvent event;
  while (XPending(display) > 0) {
    XNextEvent(display, &event);

    switch (event.type) {
    case KeyPress: {
      int xk_key = XLookupKeysym(&event.xkey, 0);
      Key pressedKey = convertXKtoKey(xk_key);
      keysPressed.push_back(pressedKey);
      break;
    }
    case KeyRelease: {
      int xk_key = XLookupKeysym(&event.xkey, 0);
      Key pressedKey = convertXKtoKey(xk_key);
      removeKeyFromKeysPressed(pressedKey);
      break;
    }

    case ConfigureNotify: {
      updateWindowSize();
      notifyAll();
      break;
    }

    default:
      break;
    }
  }
}

const std::vector<Key> &XManager::getKeyPresses() { return keysPressed; }

void XManager::clearKeyPresses() { keysPressed.clear(); }
