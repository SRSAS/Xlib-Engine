#ifndef X_MANAGER_H
#define X_MANAGER_H

#include "gameObjects.h"
#include "physicsEngine.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <memory>
#include <vector>

#define WINDOW_DEFAULT_X 0
#define WINDOW_DEFAULT_Y 0

struct Displayable {
  bool display = True;
  std::shared_ptr<DisplayVisitable> displayable;

  Displayable(std::shared_ptr<DisplayVisitable> dv);
};

enum Key {
    KEY_SPACE,
    KEY_Q,
    KEY_LEFT,
    KEY_RIGHT
};

class DisplayManager {
  virtual void addDisplayable(const DisplayVisitable &displayable);
  virtual bool removeDisplayable(std::shared_ptr<DisplayVisitable> displayable);
  virtual void setInvisible(std::shared_ptr<DisplayVisitable> displayable);
  virtual void setVisible(std::shared_ptr<DisplayVisitable> displayable);

  virtual void draw();
  virtual void erase();

  virtual void handleEvents();
  
  virtual const std::vector<Key> getKeyPresses();
  virtual void clearKeyPresses();
};

class XManager : VisitorDisplay, Observer, Observable, DisplayManager {
  Display *display;
  Window window;
  GC gc;
  int screenNum;

  std::vector<Key> keysPressed;

  int windowWidth, windowHeight;

  std::vector<std::unique_ptr<Displayable>> displayables;

  std::vector<Observer *> observers;

  XManager(int windowWidth, int windowHeight, int borderWidth);

  void visitRectangle(Rectangle &rectangle) override;

public:
  void addObserver(Observer *observer) override;
  void removeObserver(Observer *observer) override;
  void notifyAll() override;

  void onNotified() override;

  void addDisplayable(const DisplayVisitable &displayable) override;
  bool
  removeDisplayable(std::shared_ptr<DisplayVisitable> displayable) override;
  void setInvisible(std::shared_ptr<DisplayVisitable> displayable) override;
  void setVisible(std::shared_ptr<DisplayVisitable> displayable) override;

  void draw() override;
  void erase() override;

  void handleEvents() override;

  const std::vector<Key> getKeyPresses() override;
  void clearKeyPresses() override;
};

#endif
