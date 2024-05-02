#ifndef PHYSICS_H
#define PHYSICS_H

namespace physics {
struct Vector {
  int x;
  int y;

  Vector(int x, int y) : x(x), y(y) {}
};

struct Force : Vector {
  Force(int x, int y) : Vector(x, y) {}
};

struct Movement : Vector {
  Movement(int x, int y) : Vector(x, y) {}
};
} // namespace physics

#endif // !PHYSICS_H
