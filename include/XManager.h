#ifndef X_MANAGER_H
#define X_MANAGER_H

#include "gameObjects.h"
#include "physicsEngine.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <memory>
#include <vector>

#define Q_PRESSED_MASK 1
#define SPACE_PRESSED_MASK 1 << 1
#define LEFT_PRESSED_MASK 1 << 2
#define RIGHT_PRESSED_MASK 1 << 3

#define FULL_MASK std::pwd(2, 4) - 1

#define Q_UNPRESSED_MASK FULL_MASK ^ Q_PRESSED_MASK
#define SPACE_UNPRESSED_MASK FULL_MASK ^ SPACE_PRESSED_MASK
#define LEFT_UNPRESSED_MASK FULL_MASK ^ LEFT_PRESSED_MASK
#define RIGHT_UNPRESSED_MASK FULL_MASK ^ RIGHT_PRESSED_MASK

#define WINDOW_DEFAULT_X 0
#define WINDOW_DEFAULT_Y 0

struct Displayable {
  bool display = True;
  std::shared_ptr<DisplayVisitable> displayable;

  Displayable(std::shared_ptr<DisplayVisitable> dv);
};

class DisplayManager {
  virtual void addDisplayable(const DisplayVisitable &displayable);
  virtual bool removeDisplayable(std::shared_ptr<DisplayVisitable> displayable);
  virtual void setInvisible(std::shared_ptr<DisplayVisitable> displayable);
  virtual void setVisible(std::shared_ptr<DisplayVisitable> displayable);

  virtual void draw();
  virtual void erase();

  virtual void handleEvents();
};

class XManager : VisitorDisplay, Observer, Observable, DisplayManager {
  Display *display;
  Window window;
  GC gc;
  int screenNum;

  int windowWidth, windowHeight;

  std::vector<std::unique_ptr<Displayable>> displayables;

  std::vector<Observer *> observers;

  XManager(int windowWidth, int windowHeight, int borderWidth);

  void visitRectangle(Rectangle &rectangle) override;

public:
  char keyMask = 0;

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
};

#endif
