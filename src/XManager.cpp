#include "XManager.h"
#include "designPatterns.h"
#include <X11/keysym.h>
#include <cstdlib>
#include <memory>

Displayable::Displayable(std::shared_ptr<DisplayVisitable> &dv) {
  displayable = dv;
}

XManager::XManager(int windowWidth, int windowHeight, int borderWidth)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      borderWidth(borderWidth) {
  createWindow();
}

XManager::~XManager() { destroyWindow(); }

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

void XManager::destroyWindow() {
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}

void XManager::createWindow() {
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    exit(EXIT_FAILURE);
  }

  screenNum = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, screenNum), 0, 0,
                               windowWidth, windowHeight, borderWidth,
                               BlackPixel(display, screenNum),
                               BlackPixel(display, screenNum));

  XSelectInput(display, window,
               KeyPressMask | KeyReleaseMask | StructureNotifyMask);
  XMapWindow(display, window);

  gc = XCreateGC(display, window, 0, nullptr);
}

Key XManager::convertXKtoKey(int xk_key) {
  switch (xk_key) {
  case XK_q:
    return KEY_Q;
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

Key XManager::convertReleasedXKtoKey(int xk_key) {
  switch (xk_key) {
  case XK_q:
    return RELEASE_Q;
  case XK_space:
    return RELEASE_SPACE;
  case XK_Left:
    return RELEASE_LEFT;
  case XK_Right:
    return RELEASE_RIGHT;
  default:
    return NO_KEY;
  }
}

void XManager::removeKeyFromKeysPressed(Key key) {
  keysPressed.erase(std::remove(keysPressed.begin(), keysPressed.end(), key));
}

void XManager::addObserver(std::shared_ptr<Observer> observer) {
  observers.push_back(observer);
}

void XManager::removeObserver(std::shared_ptr<Observer> &observer) {
  observers.erase(std::remove(observers.begin(), observers.end(), observer));
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
  displayables.push_back(std::make_unique<Displayable>(object));
}

void XManager::setPlayer(std::shared_ptr<DisplayVisitable> player) {
  this->player = std::make_unique<Displayable>(player);
}

bool XManager::removeDisplayable(
    std::shared_ptr<DisplayVisitable> &displayable) {
  auto result = std::find_if(displayables.begin(), displayables.end(),
                             [displayable](std::unique_ptr<Displayable> &d) {
                               return d->displayable == displayable;
                             });

  if (result == displayables.end()) {
    return false;
  }

  displayables.erase(result);
  return true;
}

void XManager::removePlayer() { player = NULL; }

void XManager::setVisibility(std::shared_ptr<DisplayVisitable> &displayable,
                             bool visibile) {

  if (displayable == player->displayable) {
    player->display = visibile;
    return;
  }

  auto result = std::find_if(displayables.begin(), displayables.end(),
                             [displayable](std::unique_ptr<Displayable> &d) {
                               return d->displayable == displayable;
                             });

  if (result == displayables.end()) {
    return;
  }

  (*result)->display = visibile;
}

void XManager::setInvisible(std::shared_ptr<DisplayVisitable> &displayable) {
  setVisibility(displayable, false);
}

void XManager::setVisible(std::shared_ptr<DisplayVisitable> &displayable) {
  setVisibility(displayable, true);
}

void XManager::draw() {
  player->displayable->accept(*this);
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
      Key releasedKey = convertReleasedXKtoKey(xk_key);
      keysPressed.push_back(releasedKey);
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

void XManager::setWindowSize(int width, int height) {
  windowWidth = width;
  windowHeight = height;
  destroyWindow();
  createWindow();
}

void XManager::setBorderWidth(int width) {
  borderWidth = width;
  destroyWindow();
  createWindow();
}

int XManager::getWindowWidth() { return windowWidth; }
int XManager::getWindowHeight() { return windowHeight; }
int XManager::getBorderWidth() { return borderWidth; }
