#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include "designPatterns.h"
#include "physics.h"
#include <memory>

struct Rectangle;

struct GameObject : DisplayVisitable {
  const int id;

  physics::Position2D position;

  physics::Speed2D speed;

  physics::Acceleration2D acceleration;

  double mass;

  double width;
  double height;

  double hitboxWidth;
  double hitboxHeight;

  GameObject(int id)
      : id(id), position(0, 0), speed(0, 0), acceleration(0, 0) {}
  GameObject(int id, double width, double height, double mass);
  GameObject(int id, double width, double height, double mass, double x,
             double y);

  virtual GameObject clone();
};

typedef enum { RECTANGLE } GameObjectType;

struct Rectangle : GameObject {
  Rectangle(int id, double width, double height, double mass)
      : GameObject(id, width, height, mass) {}

  Rectangle(int id, double width, double height, double mass, double x,
            double y)
      : GameObject(id, width, height, mass, x, y) {}

  void accept(VisitorDisplay visitor) override;

  GameObject clone() override;
};

struct GameObjectFactory {
  std::shared_ptr<GameObject> createGameObject(GameObjectType type, int id);
};

#endif // !GAME_OBJECTS_H
