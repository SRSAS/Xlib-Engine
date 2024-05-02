#ifndef PHYSICS_H
#define PHYSICS_H

namespace physics {
struct Vector2D {
  double x;
  double y;

  Vector2D(double x, double y) : x(x), y(y) {}

  Vector2D operator+(const Vector2D &vector);
  Vector2D operator-(const Vector2D &vector);
  template <typename number> Vector2D operator*(const number &scalar);
  template <typename number> Vector2D operator/(const number &scalar);
};

struct Force2D : Vector2D {
  Force2D(double x, double y) : Vector2D(x, y) {}

  Force2D &operator=(const Vector2D &vector);
};

struct Acceleration2D : Vector2D {
  Acceleration2D(double x, double y) : Vector2D(x, y) {}
};

struct Speed2D : Vector2D {
  Speed2D(double x, double y) : Vector2D(x, y) {}
};

struct Position2D : Vector2D {
  Position2D(double x, double y) : Vector2D(x, y) {}
};

} // namespace physics

#endif // !PHYSICS_H
