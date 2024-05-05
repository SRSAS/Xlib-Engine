#ifndef PHYSICS_H
#define PHYSICS_H

namespace physics {
struct Vector2D {
  double x;
  double y;

  Vector2D(double x, double y) : x(x), y(y) {}
};

struct Force2D : Vector2D {
  Force2D(double x, double y) : Vector2D(x, y) {}
  template <typename V>
  Force2D(const V &vector) : Force2D(vector.x, vector.y) {}

  Force2D operator+(const Force2D &vector);
  Force2D operator-(const Force2D &vector);
  Force2D operator*(const double &scalar) const;
  Force2D operator/(const double &scalar) const;

  Force2D &operator+=(const Force2D &other);
  Force2D &operator-=(const Force2D &other);
  Force2D &operator*=(const double &scalar);
  Force2D &operator/=(const double &scalar);
};

struct Acceleration2D : Vector2D {
  Acceleration2D(double x, double y) : Vector2D(x, y) {}
  template <typename V>
  Acceleration2D(const V &vector) : Acceleration2D(vector.x, vector.y) {}

  Acceleration2D operator+(const Acceleration2D &vector);
  Acceleration2D operator-(const Acceleration2D &vector);
  Acceleration2D operator*(const double &scalar);
  Acceleration2D operator/(const double &scalar);

  Acceleration2D &operator+=(const Acceleration2D &other);
  Acceleration2D &operator-=(const Acceleration2D &other);
  Acceleration2D &operator*=(const double &scalar);
  Acceleration2D &operator/=(const double &scalar);
};

struct Speed2D : Vector2D {
  Speed2D(double x, double y) : Vector2D(x, y) {}
  template <typename V>
  Speed2D(const V &vector) : Speed2D(vector.x, vector.y) {}

  Speed2D operator+(const Speed2D &vector);
  Speed2D operator-(const Speed2D &vector);
  Speed2D operator*(const double &scalar);
  Speed2D operator/(const double &scalar);

  Speed2D &operator+=(const Speed2D &other);
  Speed2D &operator-=(const Speed2D &other);
  Speed2D &operator*=(const double &scalar);
  Speed2D &operator/=(const double &scalar);
};

struct Position2D : Vector2D {
  Position2D(double x, double y) : Vector2D(x, y) {}
  template <typename V>
  Position2D(const V &vector) : Position2D(vector.x, vector.y) {}

  Position2D operator+(const Position2D &vector);
  Position2D operator-(const Position2D &vector);
  Position2D operator*(const double &scalar);
  Position2D operator/(const double &scalar);

  Position2D &operator+=(const Position2D &other);
  Position2D &operator-=(const Position2D &other);
  Position2D &operator*=(const double &scalar);
  Position2D &operator/=(const double &scalar);
};

} // namespace physics

#endif // !PHYSICS_H
