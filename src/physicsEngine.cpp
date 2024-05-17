#include <algorithm>
#include "physicsEngine.h"
#include <chrono>
#include <memory>

#define FRAME_TIME_DIVISOR 300.0
#define BORDER_ELASTICITY 0.5 // TODO add elasticity setting to engine interface
#define FRICTION_CONSTANT 0.6

XPhysicsEngine::XPhysicsEngine(double gravityPull, double jumpImpulse,
                               double walkingSpeed, int worldWidth,
                               int worldHeight, int frameTimeDuration,
                               CollisionEngine *collisionEngine,
                               bool collisions)
    : jump(0, -jumpImpulse), gravity(0, gravityPull), walk(walkingSpeed, 0),
      worldWidth(worldWidth), worldHeight(worldHeight),
      frameTimeDuration(frameTimeDuration) {
  auto frameStartTime = std::chrono::high_resolution_clock::now();
  this->collisionEngine = std::unique_ptr<CollisionEngine>(collisionEngine);
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
  player->acceleration += force / player->mass;
}

void XPhysicsEngine::setPlayerXSpeed(double speed) { player->speed.x = speed; }

void XPhysicsEngine::setPlayerYSpeed(double speed) { player->speed.y = speed; }

void XPhysicsEngine::setPlayerSpeed(physics::Speed2D speed) {
  player->speed = speed;
}

void XPhysicsEngine::playerUpdateCoordinates() {
  objectUpdateCoordinates(player);
}
void XPhysicsEngine::playerApplyGravity() { objectApplyGravity(player); }

void XPhysicsEngine::playerApplyFloorFriction() {
  if (playerWalkingLeft || playerWalkingRight) {
    return;
  }
  objectApplyFloorFriction(player);
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

void XPhysicsEngine::objectApplyForce(std::shared_ptr<GameObject> &gameObject,
                                      physics::Force2D force) {
  gameObject->acceleration += force / gameObject->mass;
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
      physics::Speed2D(gameObject->acceleration * frameTimeElapsed.count());
  gameObject->position += physics::Position2D(
      gameObject->speed * (frameTimeElapsed.count() / FRAME_TIME_DIVISOR));

  gameObject->acceleration = physics::Acceleration2D(0, 0);

  // TODO add elasticity setting to engine interface
  if (isTouchingCeilling(gameObject)) {
    setObjectAtCeillingLevel(gameObject);
    reboundFromYAxis(gameObject);
  }
  if (isTouchingFloor(gameObject)) {
    setObjectAtFloorLevel(gameObject);
    // TODO refactor hack
    gameObject->acceleration.y = 0;
    gameObject->speed.y = 0;
  }
  if (isTouchingLeftWall(gameObject)) {
    setObjectAtLeftWallLevel(gameObject);
    reboundFromXAxis(gameObject);
  }
  if (isTouchingRightWall(gameObject)) {
    setObjectAtRightWallLevel(gameObject);
    reboundFromXAxis(gameObject);
  }

  if (collisions) {
    std::shared_ptr<GameObject> copy = gameObject->clone();
    collisionEngine->updateObjectQuadrants(copy, gameObject);

    std::vector<std::shared_ptr<GameObject>> colliders =
        collisionEngine->getCollisionsWithObject(gameObject);

    for (auto iter = colliders.begin(); iter != colliders.end(); iter++) {
      onCollision(gameObject, (*iter));
    }
  }
}

void XPhysicsEngine::objectApplyGravity(
    std::shared_ptr<GameObject> &gameObject) {
  gameObject->acceleration += gravity;
}

void XPhysicsEngine::objectApplyFloorFriction(
    std::shared_ptr<GameObject> &gameObject) {

  double initialXSpeed = gameObject->speed.x;
  if (initialXSpeed == 0.0) {
    return;
  }

  int initialSign = (initialXSpeed > 0) - (initialXSpeed < 0);
  physics::Force2D friction(0, 0);
  double maxFriction = gameObject->mass * gravity.y * FRICTION_CONSTANT;
  double forceToStopXMovement =
      ((gameObject->speed.x / frameTimeElapsed.count()) +
       gameObject->acceleration.x) *
      gameObject->mass;

  friction.x = (-initialSign) *
               std::min(std::abs(maxFriction), std::abs(forceToStopXMovement));
  objectApplyForce(gameObject, friction);
}

void XPhysicsEngine::playerSetWalkingSpeed(double speed) { walk.x = speed; }

void XPhysicsEngine::playerSetWalkingLeft() {
  //  if (!playerWalkingLeft) {
  //    walk.x = -walk.x;
  //    player->speed += walk;
  //    walk.x = -walk.x;
  //  }
  playerWalkingLeft = true;
}

void XPhysicsEngine::playerUnsetWalkingLeft() {
  //  if (playerWalkingLeft) {
  //    player->speed += walk;
  //  }
  playerWalkingLeft = false;
}

void XPhysicsEngine::playerSetWalkingRight() {
  //  if (!playerWalkingRight) {
  //    player->speed += walk;
  //  }
  playerWalkingRight = true;
}

void XPhysicsEngine::playerUnsetWalkingRight() {
  //  if (playerWalkingRight) {
  //    walk.x = -walk.x;
  //    player->speed += walk;
  //    walk.x = -walk.x;
  //  }
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

  if (playerWalkingRight) {
    player->speed.x = walk.x;
  }
  if (playerWalkingLeft) {
    player->speed.x = -walk.x;
  }
  if (playerWalkingLeft && playerWalkingRight) {
    player->speed.x = 0;
  }

  playerApplyGravity();
  playerUpdateCoordinates();
  if (isTouchingFloor(player)) {
    playerApplyFloorFriction();
  }
  //  std::cout << "Player state:" << std::endl;
  //  std::cout << "Position: X = " << player->position.x
  //            << " Y = " << player->position.y << std::endl;
  //  std::cout << "Speed: X = " << player->speed.x << " Y = " <<
  //  player->speed.y
  //            << std::endl;
  //  std::cout << "Acceleration: X = " << player->acceleration.x
  //            << " Y = " << player->acceleration.y << std::endl;
  //  std::cout << "Mass: " << player->mass << std::endl << std::endl;
  //
  for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++) {
    objectApplyGravity(*iter);
    objectUpdateCoordinates(*iter);
    objectApplyFloorFriction(*iter);
  }

  notifyAll();
}

void XPhysicsEngine::setWorldSize(int width, int height) {
  worldWidth = width;
  worldHeight = height;
  //  std::cout << "Setting world size!" << std::endl;
}
int XPhysicsEngine::getWorldWidth() { return worldWidth; }
int XPhysicsEngine::getWorldHeight() { return worldHeight; }

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
  return gameObject->position.x + gameObject->width >= worldWidth;
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
