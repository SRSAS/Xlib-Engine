#include "../include/physicsEngine.h"
#include <chrono>
#include <memory>

#define FRAME_TIME_DIVISOR 1000.0
#define BORDER_ELASTICITY 0.5 // TODO add elasticity setting to engine interface

XPhysicsEngine::XPhysicsEngine(double gravityPull, double jumpImpulse,
                               double walkingSpeed, int worldWidth,
                               int worldHeight, int frameTimeDuration,
                               CollisionEngine collisionEngine, bool collisions)
    : jump(0, -jumpImpulse), gravity(0, gravityPull), walk(walkingSpeed, 0),
      worldWidth(worldWidth), worldHeight(worldHeight),
      frameTimeDuration(frameTimeDuration), collisions(collisions) {
  auto frameStartTime = std::chrono::high_resolution_clock::now();
}

void XPhysicsEngine::setPlayer(std::shared_ptr<GameObject> player) {
  this->player = player;
}

void XPhysicsEngine::addGameObject(std::shared_ptr<GameObject> gameObject) {
  gameObjects.push_back(gameObject);
}

void XPhysicsEngine::removePlayer() { this->player = NULL; }

bool XPhysicsEngine::removeGameObject(std::shared_ptr<GameObject> &gameObject) {
  auto iter = gameObjects.begin();
  while (iter != gameObjects.end() && (*iter) != gameObject) {
    iter++;
  }
  if (iter == gameObjects.end()) {
    return false;
  }

  gameObjects.erase(iter);
  return true;
}

void XPhysicsEngine::playerJump() { playerApplyForce(jump); }

void XPhysicsEngine::setPlayerAt(physics::Position2D position) {
  player->position = position;
}

void XPhysicsEngine::playerApplyForce(physics::Force2D force) {
  auto acceleration = physics::Acceleration2D(force / player->mass);
  player->acceleration += acceleration;
}

void XPhysicsEngine::setPlayerXSpeed(double speed) { player->speed.x = speed; }

void XPhysicsEngine::setPlayerYSpeed(double speed) { player->speed.y = speed; }

void XPhysicsEngine::setPlayerSpeed(physics::Speed2D speed) {
  player->speed = speed;
}

void XPhysicsEngine::playerUpdateCoordinates() {
  objectUpdateCoordinates(player);
}

void XPhysicsEngine::objectJump(std::shared_ptr<GameObject> &gameObject) {
  auto iter = gameObjects.begin();
  while (iter != gameObjects.end() && (*iter) != gameObject) {
    iter++;
  }
  if (iter == gameObjects.end()) {
    return;
  }

  objectApplyForce((*iter), jump);
}

void XPhysicsEngine::setObjectAt(std::shared_ptr<GameObject> &gameObject,
                                 physics::Position2D position) {
  auto iter = gameObjects.begin();
  while (iter != gameObjects.end() && (*iter) != gameObject) {
    iter++;
  }
  if (iter == gameObjects.end()) {
    return;
  }

  (*iter)->position = position;
}

void XPhysicsEngine::setObjectXSpeed(std::shared_ptr<GameObject> &gameObject,
                                     double speed) {
  auto iter = gameObjects.begin();
  while (iter != gameObjects.end() && (*iter) != gameObject) {
    iter++;
  }
  if (iter == gameObjects.end()) {
    return;
  }

  (*iter)->speed.x = speed;
}

void XPhysicsEngine::setObjectYSpeed(std::shared_ptr<GameObject> &gameObject,
                                     double speed) {
  auto iter = gameObjects.begin();
  while (iter != gameObjects.end() && (*iter) != gameObject) {
    iter++;
  }
  if (iter == gameObjects.end()) {
    return;
  }

  (*iter)->speed.y = speed;
}

void XPhysicsEngine::setObjectSpeed(std::shared_ptr<GameObject> &gameObject,
                                    physics::Speed2D speed) {
  auto iter = gameObjects.begin();
  while (iter != gameObjects.end() && (*iter) != gameObject) {
    iter++;
  }
  if (iter == gameObjects.end()) {
    return;
  }
  (*iter)->speed = speed;
}

void XPhysicsEngine::objectUpdateCoordinates(
    std::shared_ptr<GameObject> &gameObject) {
  gameObject->speed +=
      physics::Speed2D(gameObject->acceleration *
                       (frameTimeElapsed.count() / FRAME_TIME_DIVISOR));
  gameObject->position += physics::Position2D(
      gameObject->speed * (frameTimeElapsed.count() / FRAME_TIME_DIVISOR));

  // TODO add elasticity setting to engine interface
  if (isTouchingCeilling(gameObject)) {
    setObjectAtCeillingLevel(gameObject);
    reboundFromYAxis(gameObject);
  }
  if (isTouchingFloor(gameObject)) {
    setObjectAtFloorLevel(gameObject);
  }
  if (isTouchingLeftWall(gameObject)) {
    setObjectAtCeillingLevel(gameObject);
    reboundFromXAxis(gameObject);
  }
  if (isTouchingRightWall(gameObject)) {
    setObjectAtCeillingLevel(gameObject);
    reboundFromXAxis(gameObject);
  }

  if (collisions) {
    GameObject copy = gameObject->clone();
    collisionEngine.updateObjectQuadrants(copy, gameObject);

    std::vector<std::shared_ptr<GameObject>> colliders =
        collisionEngine.getCollisionsWithObject(gameObject);

    for (auto iter = colliders.begin(); iter != colliders.end(); iter++) {
      onCollision(gameObject, (*iter));
    }
  }
}

void XPhysicsEngine::playerSetWalkingSpeed(double speed) { walk.x = speed; }

void XPhysicsEngine::playerSetWalkingLeft() {
  if (!playerWalkingLeft) {
    walk.x = -walk.x;
    playerApplyForce(walk);
    walk.x = -walk.x;
  }
  playerWalkingLeft = true;
}

void XPhysicsEngine::playerUnsetWalkingLeft() {
  if (playerWalkingLeft) {
    playerApplyForce(walk);
  }
  playerWalkingLeft = false;
}

void XPhysicsEngine::playerSetWalkingRight() {
  if (!playerWalkingRight) {
    playerApplyForce(walk);
  }
  playerWalkingRight = true;
}

void XPhysicsEngine::playerUnsetWalkingRight() {
  if (playerWalkingRight) {
    walk.x = -walk.x;
    playerApplyForce(walk);
    walk.x = -walk.x;
  }
  playerWalkingRight = false;
}

void XPhysicsEngine::setCollisionsOn() { collisions = true; }
void XPhysicsEngine::setCollisionsOff() { collisions = false; }

void XPhysicsEngine::addObserver(std::shared_ptr<Observer> observer) {
  observers.push_back(observer);
}

void XPhysicsEngine::removeObserver(std::shared_ptr<Observer> &observer) {
  auto iter = observers.begin();
  while (iter != observers.end() && (*iter) != observer) {
    iter++;
  }
  if (iter == observers.end()) {
    return;
  }

  observers.erase(iter);
}

void XPhysicsEngine::notifyAll() {
  for (auto iter = observers.begin(); iter != observers.end(); iter++) {
    (*iter)->onNotified();
  }
}

void XPhysicsEngine::tick() {
  frameEndTime = std::chrono::high_resolution_clock::now();
  frameTimeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
      frameEndTime - frameStartTime);

  if (frameTimeElapsed.count() <= frameTimeDuration) {
    return;
  }

  frameStartTime = std::chrono::high_resolution_clock::now();

  playerApplyForce(gravity);
  playerUpdateCoordinates();
  for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {
    objectApplyForce((*iter), gravity);
    objectUpdateCoordinates((*iter));
  }

  notifyAll();
}

bool XPhysicsEngine::isTouchingCeilling(
    std::shared_ptr<GameObject> &gameObject) {
  return gameObject->position.y <= 0;
}

bool XPhysicsEngine::isTouchingFloor(std::shared_ptr<GameObject> &gameObject) {
  return gameObject->position.y + gameObject->hitboxHeight >= worldHeight;
}

bool XPhysicsEngine::isTouchingLeftWall(
    std::shared_ptr<GameObject> &gameObject) {
  return gameObject->position.x <= 0;
}

bool XPhysicsEngine::isTouchingRightWall(
    std::shared_ptr<GameObject> &gameObject) {
  return gameObject->position.x + worldWidth >= 0;
}

void XPhysicsEngine::setObjectAtCeillingLevel(
    std::shared_ptr<GameObject> &gameObject) {
  gameObject->position.y = 0;
}

void XPhysicsEngine::setObjectAtFloorLevel(
    std::shared_ptr<GameObject> &gameObject) {
  gameObject->position.y = worldHeight - gameObject->hitboxHeight;
}

void XPhysicsEngine::setObjectAtLeftWallLevel(
    std::shared_ptr<GameObject> &gameObject) {
  gameObject->position.x = 0;
}

void XPhysicsEngine::setObjectAtRightWallLevel(
    std::shared_ptr<GameObject> &gameObject) {
  gameObject->position.x = worldWidth - gameObject->hitboxWidth;
}

void XPhysicsEngine::reboundFromYAxis(std::shared_ptr<GameObject> &gameObject) {
  gameObject->acceleration.y =
      -(BORDER_ELASTICITY * gameObject->acceleration.y);
  gameObject->speed.y = -gameObject->speed.y;
}

void XPhysicsEngine::reboundFromXAxis(std::shared_ptr<GameObject> &gameObject) {
  gameObject->acceleration.x =
      -(BORDER_ELASTICITY * gameObject->acceleration.x);
  gameObject->speed.x = -gameObject->speed.x;
}

void XPhysicsEngine::onCollision(std::shared_ptr<GameObject> &go1,
                                 std::shared_ptr<GameObject> &go2) {
  // TODO: Implement later
}
