#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include <memory>
#include "designPatterns.h"

struct Rectangle;

struct GameObject : DisplayVisitable {
  const int id;

  int x;
  int y;
  int width;
  int height;

  int hitboxWidth;
  int hitboxHeight;

  int speedX = 0;
  int speedY = 0;



  GameObject(int id): id(id) {}
};

typedef enum { RECTANGLE } GameObjectType;

struct Rectangle : GameObject {
  Rectangle(int id, int x, int y, int width, int height); 

  void accept(VisitorDisplay visitor);
};

struct GameObjectFactory {
  std::shared_ptr<GameObject> createGameObject(GameObjectType type, int id);
};

#endif // !GAME_OBJECTS_H
