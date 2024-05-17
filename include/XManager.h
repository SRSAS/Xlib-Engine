#ifndef X_MANAGER_H
#define X_MANAGER_H

#include "designPatterns.h"
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

  Displayable(std::shared_ptr<DisplayVisitable> &dv);
};

enum Key {
  NO_KEY,
  KEY_SPACE,
  RELEASE_SPACE,
  KEY_Q,
  RELEASE_Q,
  KEY_LEFT,
  RELEASE_LEFT,
  KEY_RIGHT,
  RELEASE_RIGHT
};

class DisplayManager : public VisitorDisplay,
                       public Observer,
                       public Observable {
public:
  virtual void addDisplayable(std::shared_ptr<DisplayVisitable> object) = 0;
  virtual void setPlayer(std::shared_ptr<DisplayVisitable> player) = 0;

  virtual bool
  removeDisplayable(std::shared_ptr<DisplayVisitable> &displayable) = 0;
  virtual void removePlayer() = 0;
  virtual void setInvisible(std::shared_ptr<DisplayVisitable> &displayable) = 0;
  virtual void setVisible(std::shared_ptr<DisplayVisitable> &displayable) = 0;

  virtual void draw() = 0;
  virtual void erase() = 0;

  virtual void handleEvents() = 0;

  virtual const std::vector<Key> &getKeyPresses() = 0;
  virtual void clearKeyPresses() = 0;

  virtual void setWindowSize(int width, int height) = 0;
  virtual void setBorderWidth(int width) = 0;
  virtual int getWindowWidth() = 0;
  virtual int getWindowHeight() = 0;
  virtual int getBorderWidth() = 0;
};

class XManager : public DisplayManager {
  Display *display;
  Window window;
  GC gc;
  int screenNum;

  std::vector<Key> keysPressed;

  std::unique_ptr<Displayable> player;
  std::vector<std::unique_ptr<Displayable>> displayables;

  std::vector<std::shared_ptr<Observer>> observers;

  void visitRectangle(const Rectangle &rectangle) override;

  void updateWindowSize();

  void destroyWindow();
  void createWindow();

  Key convertXKtoKey(int xk_key);
  Key convertReleasedXKtoKey(int xk_key);
  void removeKeyFromKeysPressed(Key key);
  void setVisibility(std::shared_ptr<DisplayVisitable> &displayable,
                     bool visibility);

public:
  XManager(int windowWidth, int windowHeight, int borderWidth);
  ~XManager();

  int windowWidth, windowHeight, borderWidth;

  void addObserver(std::shared_ptr<Observer> observer) override;
  void removeObserver(std::shared_ptr<Observer> &observer) override;
  void notifyAll() override;

  void onNotified() override;

  void addDisplayable(std::shared_ptr<DisplayVisitable> object) override;
  void setPlayer(std::shared_ptr<DisplayVisitable> player) override;
  bool
  removeDisplayable(std::shared_ptr<DisplayVisitable> &displayable) override;
  void removePlayer() override;
  void setInvisible(std::shared_ptr<DisplayVisitable> &displayable) override;
  void setVisible(std::shared_ptr<DisplayVisitable> &displayable) override;

  void draw() override;
  void erase() override;

  void handleEvents() override;

  const std::vector<Key> &getKeyPresses() override;
  void clearKeyPresses() override;

  void setWindowSize(int width, int height) override;
  int getWindowWidth() override;
  int getWindowHeight() override;

  void setBorderWidth(int width) override;
  int getBorderWidth() override;
};

#endif
