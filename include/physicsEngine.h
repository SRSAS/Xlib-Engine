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
  virtual void setPlayerAt(physics::Position2D position);
  virtual void playerApplyForce(physics::Force2D force);
  virtual void setPlayerXSpeed(int speed);
  virtual void setPlayerYSpeed(int speed);
  virtual void setPlayerSpeed(physics::Speed2D);

  virtual void objectJump(std::shared_ptr<GameObject> &gameObject);
  virtual void setObjectAt(std::shared_ptr<GameObject> &gameObject,
                           physics::Position2D position);
  virtual void objectApplyForce(std::shared_ptr<GameObject> &gameObject,
                                physics::Force2D force);
  virtual void setObjectXSpeed(std::shared_ptr<GameObject> &gameObject,
                               int speed);
  virtual void setObjectYSpeed(std::shared_ptr<GameObject> &gameObject,
                               int speed);
  virtual void setObjectSpeed(std::shared_ptr<GameObject> &gameObject,
                              physics::Speed2D);

  // Player movement utilities
  virtual void playerSetWalkingSpeed(int speed);
  virtual void playerSetWalkingLeft();
  virtual void playerUnsetWalkingLeft();
  virtual void playerSetWalkingRight();
  virtual void playerUnsetWalkingRight();

  virtual void tick();
};

struct XPhysicsEngine : Observable, PhysicsEngine {
  physics::Force2D gravity;
  physics::Force2D jump;

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
  void setPlayerAt(physics::Position2D position) override;
  void playerApplyForce(physics::Force2D force) override;
  void setPlayerXSpeed(int speed) override;
  void setPlayerYSpeed(int speed) override;
  void setPlayerSpeed(physics::Speed2D) override;

  void objectJump(std::shared_ptr<GameObject> &gameObject) override;
  void setObjectAt(std::shared_ptr<GameObject> &gameObject,
                   physics::Position2D position) override;
  void objectApplyForce(std::shared_ptr<GameObject> &gameObject,
                        physics::Force2D force) override;
  void setObjectXSpeed(std::shared_ptr<GameObject> &gameObject,
                       int speed) override;
  void setObjectYSpeed(std::shared_ptr<GameObject> &gameObject,
                       int speed) override;
  void setObjectSpeed(std::shared_ptr<GameObject> &gameObject,
                      physics::Speed2D) override;

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
