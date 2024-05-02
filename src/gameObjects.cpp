#include "../include/gameObjects.h"
#include <memory>

Rectangle::Rectangle(int id, int x, int y, int width, int height)
    : GameObject(id) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

void Rectangle::accept(VisitorDisplay visitor) {
  visitor.visitRectangle(*this);
}

std::shared_ptr<GameObject>
GameObjectFactory::createGameObject(GameObjectType type, int id) {
  switch (type) {
  case RECTANGLE:
    return std::make_shared<Rectangle>(0, 0, 0, 0);
  default:
    return NULL;
  }
}
