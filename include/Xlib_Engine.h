#ifndef XLIB_ENGINE_H
#define XLIB_ENGINE_H

#include "XManager.h"
#include "gameObjects.h"
#include "physicsEngine.h"
#include <functional>
#include <memory>

class GameEngine : Observable {
  DisplayManager displayManager;
  PhysicsEngine physicsEngine;

  std::vector<std::shared_ptr<GameObject>> gameObjects;
  int gameObjectInstantiationCount = 0;

  std::vector<Observer *> observers;

  void handleKeyPresses();

public:
  GameEngine(int windowWidth, int windowHeight, int borderWidth);

  /**
   * Start the event loop.
   * On each iteration, call tick() on physicsEngine, and handleEvents() on
   * DisplayManager
   */
  void run();

  void notifyAll() override;
  void addObserver(std::shared_ptr<Observer> observer) override;
  void removeObserver(std::shared_ptr<Observer> &observer) override;

  void setNewPlayer(GameObjectType, int x, int y, int width, int height);
  int addNewObject(GameObjectType, int x, int y, int width, int height);

  void removePlayer();
  bool removeGameObject(int objectID);

  void playerJump();
  void setPlayerAt(int x, int y);
  void playerApplyForce(int x, int y);
  void playerSetXSpeed(int x);
  void playerSetYSpeed(int y);
  void playerSetSpeed(int x, int y);

  void jumpObject(int objectID);
  void objectSetAt(int objectID, int x, int y);
  void objectApplyForce(int objectID, int x, int y);
  void objectSetXSpeed(int objectID, int x);
  void objectSetYSpeed(int objectID, int y);
  void objectSetSpeed(int objectID, int x, int y);

  // Player movement utilities
  void playerSetWalkingSpeed(int speed);
  void playerSetWalkingLeft();
  void playerUnsetWalkingLeft();
  void playerSetWalkingRight();
  void playerUnsetWalkingRight();

  int addSprite(GameObjectType type, int x, int y, int width, int height);
  bool removeSprite(int objectID);
  void setInvisible(int objectID);
  void setVisible(int objectID);

  void onKeyPressed(Key key,
                    std::function<void(const GameEngine &)> keyHandler);
};

#endif // !XLIB_ENGINE_H
