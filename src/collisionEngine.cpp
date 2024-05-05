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

bool MockCollisionEngine::objectsCollided(
    const std::shared_ptr<GameObject> &o1,
    const std::shared_ptr<GameObject> &o2) {
  return false;
}

void MockCollisionEngine::addGameObject(
    std::shared_ptr<GameObject> gameObject) {}

void MockCollisionEngine::removeGameObject(
    std::shared_ptr<GameObject> &gameObject) {}

void MockCollisionEngine::updateObjectQuadrants(
    std::shared_ptr<GameObject> &previousState,
    std::shared_ptr<GameObject> &newState) {}
