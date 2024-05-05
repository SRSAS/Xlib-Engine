#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "collisionEngine.h"
#include "designPatterns.h"
#include "gameObjects.h"
#include "physics.h"
#include <chrono>
#include <memory>
#include <vector>

struct PhysicsEngine : Observable {
  // Add game objects
  virtual void setPlayer(std::shared_ptr<GameObject> player) = 0;
  virtual void addGameObject(std::shared_ptr<GameObject> gameObject) = 0;

  // Remove game objects
  virtual void removePlayer() = 0;
  /**
   * Remove the game object with the given ID
   *
   * @param gameObject shared_ptr to game object
   * @return True if the object existed, False otherwise
   */
  virtual bool removeGameObject(std::shared_ptr<GameObject> &gameObject) = 0;

  // Objects movement
  virtual void playerJump() = 0;
  virtual void setPlayerAt(physics::Position2D position) = 0;
  virtual void playerApplyForce(physics::Force2D force) = 0;
  virtual void setPlayerXSpeed(double speed) = 0;
  virtual void setPlayerYSpeed(double speed) = 0;
  virtual void setPlayerSpeed(physics::Speed2D) = 0;
  virtual void playerUpdateCoordinates() = 0;

  virtual void objectJump(std::shared_ptr<GameObject> &gameObject) = 0;
  virtual void setObjectAt(std::shared_ptr<GameObject> &gameObject,
                           physics::Position2D position) = 0;
  virtual void objectApplyForce(std::shared_ptr<GameObject> &gameObject,
                                physics::Force2D force) = 0;
  virtual void setObjectXSpeed(std::shared_ptr<GameObject> &gameObject,
                               double speed) = 0;
  virtual void setObjectYSpeed(std::shared_ptr<GameObject> &gameObject,
                               double speed) = 0;
  virtual void setObjectSpeed(std::shared_ptr<GameObject> &gameObject,
                              physics::Speed2D) = 0;
  virtual void
  objectUpdateCoordinates(std::shared_ptr<GameObject> &gameObject) = 0;

  // Player movement utilities
  virtual void playerSetWalkingSpeed(double speed) = 0;
  virtual void playerSetWalkingLeft() = 0;
  virtual void playerUnsetWalkingLeft() = 0;
  virtual void playerSetWalkingRight() = 0;
  virtual void playerUnsetWalkingRight() = 0;

  virtual void tick() = 0;

  virtual void setWorldSize(int width, int height) = 0;
  virtual int getWorldWidth() = 0;
  virtual int getWorldHeight() = 0;
};

struct XPhysicsEngine :  PhysicsEngine {
  physics::Force2D gravity;
  physics::Force2D jump;
  physics::Force2D walk;

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

  bool playerWalkingLeft = false;
  bool playerWalkingRight = false;

  std::vector<std::shared_ptr<Observer>> observers;

  std::unique_ptr<CollisionEngine> collisionEngine;
  bool collisions = false;

public:
  XPhysicsEngine(double gravityPull, double jumpImpulse, double walkingSpeed,
                 int worldWidth, int worldHeight, int frameTimeDuration,
                 CollisionEngine *collisionEngine, bool collisions);

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
  void setPlayerXSpeed(double speed) override;
  void setPlayerYSpeed(double speed) override;
  void setPlayerSpeed(physics::Speed2D) override;
  void playerUpdateCoordinates() override;

  void objectJump(std::shared_ptr<GameObject> &gameObject) override;
  void setObjectAt(std::shared_ptr<GameObject> &gameObject,
                   physics::Position2D position) override;
  void objectApplyForce(std::shared_ptr<GameObject> &gameObject,
                        physics::Force2D force) override;
  void setObjectXSpeed(std::shared_ptr<GameObject> &gameObject,
                       double speed) override;
  void setObjectYSpeed(std::shared_ptr<GameObject> &gameObject,
                       double speed) override;
  void setObjectSpeed(std::shared_ptr<GameObject> &gameObject,
                      physics::Speed2D) override;
  void
  objectUpdateCoordinates(std::shared_ptr<GameObject> &gameObject) override;

  // Player movement utilities
  void playerSetWalkingSpeed(double speed) override;
  void playerSetWalkingLeft() override;
  void playerUnsetWalkingLeft() override;
  void playerSetWalkingRight() override;
  void playerUnsetWalkingRight() override;

  // Collisions
  void setCollisionsOn();
  void setCollisionsOff();

  // Observable pattern
  void addObserver(std::shared_ptr<Observer> observer) override;
  void removeObserver(std::shared_ptr<Observer> &observer) override;
  void notifyAll() override;

  // Event Loop
  /**
   * Called every iteration of the event cycle.
   * Count frame time elapse. On frameTimeElapsed >= frameTimeDuration a
   * frame has passed and frameStartTime is reset.
   * On each frame, update game objects and notify all observers.
   */
  void tick() override;

  void setWorldSize(int width, int height) override;
  int getWorldWidth() override;
  int getWorldHeight() override;

private:
  bool isTouchingCeilling(std::shared_ptr<GameObject> &gameObject);
  bool isTouchingFloor(std::shared_ptr<GameObject> &gameObject);
  bool isTouchingLeftWall(std::shared_ptr<GameObject> &gameObject);
  bool isTouchingRightWall(std::shared_ptr<GameObject> &gameObject);

  void setObjectAtCeillingLevel(std::shared_ptr<GameObject> &gameObject);
  void setObjectAtFloorLevel(std::shared_ptr<GameObject> &gameObject);
  void setObjectAtLeftWallLevel(std::shared_ptr<GameObject> &gameObject);
  void setObjectAtRightWallLevel(std::shared_ptr<GameObject> &gameObject);

  void reboundFromYAxis(std::shared_ptr<GameObject> &gameObject);
  void reboundFromXAxis(std::shared_ptr<GameObject> &gameObject);

  void onCollision(std::shared_ptr<GameObject> &go1,
                   std::shared_ptr<GameObject> &go2);
};

#endif // !PHYSICS_ENGINE_H
