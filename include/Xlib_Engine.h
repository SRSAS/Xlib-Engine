#ifndef XLIB_ENGINE_H
#define XLIB_ENGINE_H

#include "XManager.h"
#include "gameObjects.h"
#include "physicsEngine.h"
#include <functional>
#include <memory>
#include <unordered_map>

class GameEngine;

struct WindowChangeObserver : Observer {
  GameEngine *gameEngine;

  WindowChangeObserver(GameEngine *gameEngine) : gameEngine(gameEngine) {}

  void onNotified() override;
};

struct FrameObserver : Observer {
  GameEngine *gameEngine;

  FrameObserver(GameEngine *gameEngine) : gameEngine(gameEngine) {}

  void onNotified() override;
};

class GameEngine : Observable {
  DisplayManager *displayManager;
  PhysicsEngine *physicsEngine;

  GameObjectFactory gameObjectFactory;
  std::shared_ptr<GameObject> player;
  std::vector<std::shared_ptr<GameObject>> gameObjects;
  int gameObjectInstantiationCount = 0;

  std::unordered_map<Key, std::function<void(GameEngine &)>> keyHandlers;

  std::vector<std::shared_ptr<Observer>> observers;
  FrameObserver frameObserver;
  WindowChangeObserver windowChangeObserver;

  bool exitFlag;

  void handleKeyPresses();

  std::shared_ptr<GameObject> createNewGameObject(GameObjectType type, int x,
                                                  int y, int width, int height,
                                                  int mass);
  std::shared_ptr<GameObject> &getObjectByID(int objectID);

public:
  GameEngine(int windowWidth, int windowHeight, int borderWidth,
             double gravitationalPull, double jumpImpulse, double walkingSpeed,
             int frameDuration, bool collisions);

  ~GameEngine();

  void updateWorldSize();

  /**
   * Start the event loop.
   * On each iteration, call tick() on physicsEngine, and handleEvents() on
   * DisplayManager
   */
  void run();
  void exit();

  void notifyAll() override;
  void addObserver(std::shared_ptr<Observer> observer) override;
  void removeObserver(std::shared_ptr<Observer> &observer) override;

  void setNewPlayer(GameObjectType type, int x, int y, int width, int height,
                    int mass);
  int addNewObject(GameObjectType type, int x, int y, int width, int height,
                   int mass);

  void removePlayer();
  bool removeGameObject(int objectID);

  void playerJump();
  void setPlayerAt(int x, int y);
  void playerApplyForce(double x, double y);
  void playerSetXSpeed(double x);
  void playerSetYSpeed(double y);
  void playerSetSpeed(double x, double y);

  void jumpObject(int objectID);
  void objectSetAt(int objectID, int x, int y);
  void objectApplyForce(int objectID, double x, double y);
  void objectSetXSpeed(int objectID, double x);
  void objectSetYSpeed(int objectID, double y);
  void objectSetSpeed(int objectID, double x, double y);

  // Player movement utilities
  void playerSetWalkingSpeed(int speed);
  void playerSetWalkingLeft();
  void playerUnsetWalkingLeft();
  void playerSetWalkingRight();
  void playerUnsetWalkingRight();

  int addSprite(GameObjectType type, int x, int y, int width, int height,
                int mass);
  bool removeSprite(int objectID);
  void setInvisible(int objectID);
  void setVisible(int objectID);

  void onKeyPressed(Key key, std::function<void(GameEngine &)> keyHandler);
};

#endif // !XLIB_ENGINE_H
