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
  void addObserver(Observer *observer) override;
  void removeObserver(Observer *observer) override;

  void setNewPlayer(GameObjectType, int x, int y, int width, int height);
  int addNewObject(GameObjectType, int x, int y, int width, int height);

  void removePlayer();
  bool removeGameObject(int objectID);

  void playerJump();
  void playerMove(int x, int y);
  void setPlayerAt(int x, int y);
  void playerApplyForce(int x, int y);

  void stopPlayerMovement();
  void stopPlayerXMovement();
  void stopPlayerYMovement();

  void jumpObject(int objectID);
  void moveObject(int objectID, int x, int y);
  void setObjectAt(int objectID, int x, int y);
  void objectApplyForce(int objectID, int x, int y);

  void stopObjectMovement(int objectID);
  void stopObjectXMovement(int objectID);
  void stopObjectYMovement(int objectID);

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

  void onKeyPressed(Key key, std::function<void(const GameEngine&)> keyHandler);
};

#endif // !XLIB_ENGINE_H
