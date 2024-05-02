#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "collisionEngine.h"
#include "designPatterns.h"
#include "gameObjects.h"
#include "physics.h"
#include <chrono>
#include <memory>
#include <vector>

struct PhysicsEngine {
  // Add game objects
  virtual void setPlayer(std::shared_ptr<GameObject> player);
  virtual void addGameObject(std::shared_ptr<GameObject> gameObject);

  // Remove game objects
  virtual void removePlayer();
  /**
   * Remove the game object with the given ID
   *
   * @param gameObject shared_ptr to game object
   * @return True if the object existed, False otherwise
   */
  virtual bool removeGameObject(std::shared_ptr<GameObject> &gameObject);

  // Objects movement
  virtual void playerJump();
  virtual void playerMove(physics::Movement movement);
  virtual void setPlayerAt(int x, int y);
  virtual void playerApplyForce(physics::Force force);

  virtual void stopPlayerMovement();
  virtual void stopPlayerXMovement();
  virtual void stopPlayerYMovement();

  virtual void jumpObject(std::shared_ptr<GameObject> &gameObject);
  virtual void moveObject(std::shared_ptr<GameObject> &gameObject,
                          physics::Movement movement);
  virtual void setObjectAt(std::shared_ptr<GameObject> &gameObject, int x,
                           int y);
  virtual void objectApplyForce(std::shared_ptr<GameObject> &gameObject,
                                physics::Force force);

  virtual void stopObjectMovement(std::shared_ptr<GameObject> &gameObject);
  virtual void stopObjectXMovement(std::shared_ptr<GameObject> &gameObject);
  virtual void stopObjectYMovement(std::shared_ptr<GameObject> &gameObject);

  // Player movement utilities
  virtual void playerSetWalkingSpeed(int speed);
  virtual void playerSetWalkingLeft();
  virtual void playerUnsetWalkingLeft();
  virtual void playerSetWalkingRight();
  virtual void playerUnsetWalkingRight();

  virtual void tick();
};

struct XPhysicsEngine : Observable, PhysicsEngine {
  physics::Force gravity;
  physics::Force jump;

  int worldWidth;
  int worldHeight;

  bool ceillingWall = true;
  bool leftWall = true;
  bool rightWall = true;

  int frameTimeDuration;

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> frameStartTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> frameEndTime;
  std::chrono::milliseconds frameTimeElapsed;

  std::shared_ptr<GameObject> player;
  std::vector<std::shared_ptr<GameObject>> gameObjects;

  int playerWalkingSpeed;
  bool playerWalkingLeft = false;
  bool playerWalkingRight = false;

  std::vector<Observer *> observers;

  CollisionEngine collisionEngine;
  bool collisions = false;

public:
  XPhysicsEngine(int gravityPull, int jumpImpulse, int worldWidth,
                 int worldHeight, int frameTimeDuration);

  // Add game objects
  void setPlayer(std::shared_ptr<GameObject> player) override;
  void addGameObject(std::shared_ptr<GameObject> gameObject) override;

  // Remove game objects
  void removePlayer() override;
  /**
   * Remove the game object with the given ID
   *
   * @param gameObject shared_ptr to game object
   * @return True if the object existed, False otherwise
   */
  bool removeGameObject(std::shared_ptr<GameObject> &gameObject) override;

  // Objects movement
  void playerJump() override;
  void playerMove(physics::Movement movement) override;
  void setPlayerAt(int x, int y) override;
  void playerApplyForce(physics::Force force) override;

  void stopPlayerMovement() override;
  void stopPlayerXMovement() override;
  void stopPlayerYMovement() override;

  void jumpObject(std::shared_ptr<GameObject> &gameObject) override;
  void moveObject(std::shared_ptr<GameObject> &gameObject,
                  physics::Movement movement) override;
  void setObjectAt(std::shared_ptr<GameObject> &gameObject, int x,
                   int y) override;
  void objectApplyForce(std::shared_ptr<GameObject> &gameObject,
                        physics::Force force) override;

  void stopObjectMovement(std::shared_ptr<GameObject> &gameObject) override;
  void stopObjectXMovement(std::shared_ptr<GameObject> &gameObject) override;
  void stopObjectYMovement(std::shared_ptr<GameObject> &gameObject) override;

  // Player movement utilities
  void playerSetWalkingSpeed(int speed) override;
  void playerSetWalkingLeft() override;
  void playerUnsetWalkingLeft() override;
  void playerSetWalkingRight() override;
  void playerUnsetWalkingRight() override;

  // Observable pattern
  void addObserver(Observer *observer) override;
  void removeObserver(Observer *observer) override;
  void notifyAll() override;

  // Event Loop
  /**
   * Called every iteration of the event cycle.
   * Count frame time elapse. On frameTimeElapsed >= frameTimeDuration a
   * frame has passed and frameStartTime is reset.
   * On each frame, update game objects and notify all observers.
   */
  void tick() override;

private:
  void notify();

  bool isTouchingCeilling(std::shared_ptr<GameObject> &gameObject);
  bool isTouchingFloor(std::shared_ptr<GameObject> &gameObject);
  bool isTouchingLeftWall(std::shared_ptr<GameObject> &gameObject);
  bool isTouchingRightWall(std::shared_ptr<GameObject> &gameObject);

  void setObjectAtCeillingLevel(std::shared_ptr<GameObject> &gameObject);
  void setObjectAtFloorLevel(std::shared_ptr<GameObject> &gameObject);
  void setObjectAtLeftWallLevel(std::shared_ptr<GameObject> &gameObject);
  void setObjectAtRightWallLevel(std::shared_ptr<GameObject> &gameObject);

  void onCollision(std::shared_ptr<GameObject> &go1,
                   std::shared_ptr<GameObject> &go2);
};

#endif // !PHYSICS_ENGINE_H
