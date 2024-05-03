#include "../include/collisionEngine.h"
#include <memory>
#include <vector>

std::vector<Collision> MockCollisionEngine::getAllCollisions() {
  std::vector<Collision> empty;
  return empty;
}

std::vector<std::shared_ptr<GameObject>>
MockCollisionEngine::getCollisionsWithObject(
    std::shared_ptr<GameObject> &gameObject) {
  std::vector<std::shared_ptr<GameObject>> empty;
  return empty;
}

void MockCollisionEngine::addGameObject(
    std::shared_ptr<GameObject> gameObject) {}

void MockCollisionEngine::removeGameObject(
    std::shared_ptr<GameObject> &gameObject) {}

void MockCollisionEngine::updateObjectQuadrants(
    GameObject &previousState, std::shared_ptr<GameObject> &newState) {}
