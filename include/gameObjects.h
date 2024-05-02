#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include "designPatterns.h"
#include <memory>

struct Rectangle;

struct GameObject : DisplayVisitable {
  const double id;

  double x;
  double y;
  double width;
  double height;

  double hitboxWidth;
  double hitboxHeight;

  double speedX = 0;
  double speedY = 0;

  double accelerationX = 0;
  double accelerationY = 0;

  double mass;

  GameObject(double id) : id(id) {}
};

typedef enum { RECTANGLE } GameObjectType;

struct Rectangle : GameObject {
  Rectangle(double id, double x, double y, double width, double height,
            double mass);

  void accept(VisitorDisplay visitor);
};

struct GameObjectFactory {
  std::shared_ptr<GameObject> createGameObject(GameObjectType type, double id);
};

#endif // !GAME_OBJECTS_H
