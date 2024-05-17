#include "gameObjects.h"
#include <memory>

GameObject::GameObject(int id, double width, double height, double mass)
    : GameObject(id) {
  this->width = width;
  this->height = height;
  this->mass = mass;
}

GameObject::GameObject(int id, double width, double height, double mass,
                       double x, double y)
    : GameObject(id, width, height, mass) {
  this->position.x = x;
  this->position.y = y;
}

void Rectangle::accept(VisitorDisplay &visitor) {
  visitor.visitRectangle(*this);
}

std::shared_ptr<GameObject> Rectangle::clone() {
  Rectangle copy(*this);
  return std::make_shared<Rectangle>(copy);
}

std::shared_ptr<GameObject>
GameObjectFactory::createGameObject(GameObjectType type, int id) {
  switch (type) {
  case RECTANGLE:
    return std::make_shared<Rectangle>(id, 0, 0, 0, 0, 0);
  default:
    return NULL;
  }
}
