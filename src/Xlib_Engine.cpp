#include "../include/Xlib_Engine.h"
#include <memory>

void WindowChangeObserver::onNotified() { gameEngine->updateWorldSize(); }

void FrameObserver::onNotified() { gameEngine->notifyAll(); }

GameEngine::GameEngine(int windowWidth, int windowHeight, int borderWidth,
                       double gravitationalPull, double jumpImpulse,
                       double walkingSpeed, int frameDuration,
                       bool collisions) {
  auto collisionEngine = new MockCollisionEngine();
  displayManager = new XManager(windowWidth, windowHeight, borderWidth);
  physicsEngine = new XPhysicsEngine(
      gravitationalPull, jumpImpulse, walkingSpeed, windowWidth, windowHeight,
      frameDuration, collisionEngine, collisions);
}

GameEngine::~GameEngine() {
  delete displayManager;
  delete physicsEngine;
}

void GameEngine::updateWorldSize() {
  physicsEngine->setWorldSize(displayManager->getWindowWidth(),
                              displayManager->getWindowHeight());
}

void GameEngine::handleKeyPresses() {
  const std::vector<Key> keysPressed = displayManager->getKeyPresses();
  for (auto iter = keysPressed.begin(); iter != keysPressed.end(); iter++) {
    auto handler = keyHandlers.at((*iter));
    handler(*this);
  }
  displayManager->clearKeyPresses();
}

std::shared_ptr<GameObject>
GameEngine::createNewGameObject(GameObjectType type, int x, int y, int width,
                                int height, int mass) {
  std::shared_ptr<GameObject> gameObject =
      gameObjectFactory.createGameObject(type, gameObjectInstantiationCount++);

  gameObject->position.x = x;
  gameObject->position.y = y;

  gameObject->width = width;
  gameObject->height = height;

  // TODO perchance change later?
  gameObject->hitboxWidth = width;
  gameObject->hitboxHeight = height;

  gameObject->mass = mass;

  return gameObject;
}

std::shared_ptr<GameObject> &GameEngine::getObjectByID(int objectID) {
  static std::shared_ptr<GameObject> nullPtr;
  auto iter = gameObjects.begin();
  while (iter != gameObjects.end() && (*iter)->id != objectID) {
    iter++;
  }
  if (iter == gameObjects.end()) {
    return nullPtr;
  }

  return *iter;
}

void GameEngine::run() {
  while (!exitFlag) {
    displayManager->handleEvents();
    handleKeyPresses();
    physicsEngine->tick();
  }
}

void GameEngine::exit() { exitFlag = true; }

void GameEngine::notifyAll() {
  for (auto iter = observers.begin(); iter != observers.end(); iter++) {
    (*iter)->onNotified();
  }
}

void GameEngine::addObserver(std::shared_ptr<Observer> observer) {
  observers.push_back(observer);
}

void GameEngine::removeObserver(std::shared_ptr<Observer> &observer) {
  auto iter = observers.begin();
  while (iter != observers.end() && (*iter) != observer) {
    iter++;
  }
  if (iter == observers.end()) {
    return;
  }

  observers.erase(iter);
}

void GameEngine::setNewPlayer(GameObjectType type, int x, int y, int width,
                              int height, int mass) {
  player = createNewGameObject(type, x, y, width, height, mass);

  displayManager->setPlayer(player);
  physicsEngine->setPlayer(player);
}

int GameEngine::addNewObject(GameObjectType type, int x, int y, int width,
                             int height, int mass) {
  std::shared_ptr<GameObject> gameObject =
      createNewGameObject(type, x, y, width, height, mass);

  gameObjects.push_back(gameObject);
  displayManager->addDisplayable(gameObject);
  physicsEngine->addGameObject(gameObject);

  return gameObject->id;
}

void GameEngine::removePlayer() {
  physicsEngine->removePlayer();
  displayManager->removePlayer();
  player = NULL;
}

bool GameEngine::removeGameObject(int objectID) {
  auto iter = gameObjects.begin();
  while (iter != gameObjects.end() && (*iter)->id != objectID) {
    iter++;
  }
  if (iter == gameObjects.end()) {
    return false;
  }

  std::shared_ptr<DisplayVisitable> displayVisitable = *iter;

  gameObjects.erase(iter);
  displayManager->removeDisplayable(displayVisitable);
  physicsEngine->removeGameObject(*iter);

  return true;
}

void GameEngine::playerJump() { physicsEngine->playerJump(); }

void GameEngine::setPlayerAt(int x, int y) {
  physics::Position2D position(x, y);
  physicsEngine->setPlayerAt(position);
}

void GameEngine::playerApplyForce(double x, double y) {
  physics::Force2D force(x, y);
  physicsEngine->playerApplyForce(force);
}

void GameEngine::playerSetXSpeed(double x) {
  physicsEngine->setPlayerXSpeed(x);
}

void GameEngine::playerSetYSpeed(double y) {
  physicsEngine->setPlayerYSpeed(y);
}

void GameEngine::playerSetSpeed(double x, double y) {
  physics::Speed2D speed(x, y);
  physicsEngine->setPlayerSpeed(speed);
}

void GameEngine::jumpObject(int objectID) {
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    physicsEngine->objectJump(gameObject);
  }
}

void GameEngine::objectSetAt(int objectID, int x, int y) {
  physics::Position2D position(x, y);
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    physicsEngine->setObjectAt(gameObject, position);
  }
}

void GameEngine::objectApplyForce(int objectID, double x, double y) {
  physics::Force2D force(x, y);
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    physicsEngine->objectApplyForce(gameObject, force);
  }
}

void GameEngine::objectSetXSpeed(int objectID, double x) {
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    physicsEngine->setObjectXSpeed(gameObject, x);
  }
}

void GameEngine::objectSetYSpeed(int objectID, double y) {
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    physicsEngine->setObjectYSpeed(gameObject, y);
  }
}

void GameEngine::objectSetSpeed(int objectID, double x, double y) {
  physics::Speed2D speed(x, y);
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    physicsEngine->setObjectSpeed(gameObject, speed);
  }
}

void GameEngine::playerSetWalkingSpeed(int speed) {
  physicsEngine->playerSetWalkingSpeed(speed);
}

void GameEngine::playerSetWalkingLeft() {
  physicsEngine->playerSetWalkingLeft();
}

void GameEngine::playerSetWalkingRight() {
  physicsEngine->playerSetWalkingRight();
}

void GameEngine::playerUnsetWalkingRight() {
  physicsEngine->playerUnsetWalkingRight();
}

void GameEngine::playerUnsetWalkingLeft() {
  physicsEngine->playerUnsetWalkingLeft();
}

int GameEngine::addSprite(GameObjectType type, int x, int y, int width,
                          int height, int mass) {
  std::shared_ptr<GameObject> gameObject =
      createNewGameObject(type, x, y, width, height, mass);

  gameObjects.push_back(gameObject);
  displayManager->addDisplayable(gameObject);

  return gameObject->id;
}

bool GameEngine::removeSprite(int objectID) {
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    std::shared_ptr<DisplayVisitable> displayVisitable = gameObject;
    displayManager->removeDisplayable(displayVisitable);
    return true;
  }
  return false;
}

void GameEngine::setInvisible(int objectID) {
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    std::shared_ptr<DisplayVisitable> displayVisitable = gameObject;
    displayManager->setInvisible(displayVisitable);
  }
}

void GameEngine::setVisible(int objectID) {
  std::shared_ptr<GameObject> gameObject = getObjectByID(objectID);
  if (gameObject) {
    std::shared_ptr<DisplayVisitable> displayVisitable = gameObject;
    displayManager->setVisible(displayVisitable);
  }
}

void GameEngine::onKeyPressed(Key key,
                              std::function<void(GameEngine &)> keyHandler) {
  keyHandlers.emplace(key, keyHandler);
}
