#include "../include/physics.h"

using namespace physics;

Vector2D Vector2D::operator+(const Vector2D &vector) {
  Vector2D sum(0, 0);
  sum.x = x + vector.x;
  sum.y = y + vector.y;
  return sum;
}

Vector2D Vector2D::operator-(const Vector2D &vector) {
  Vector2D sub(0, 0);
  sub.x = x - vector.x;
  sub.y = y - vector.y;
  return sub;
}

template <typename number>
Vector2D Vector2D::operator*(const number &scalar) {
  Vector2D mul(0, 0);
  mul.x = x * scalar;
  mul.y = y * scalar;
  return mul;
}

template <typename number>
Vector2D Vector2D::operator/(const number &scalar) {
  Vector2D div(0, 0);
  div.x = x / scalar;
  div.y = y / scalar;
  return div;
}

Force2D& Force2D::operator=(const Vector2D &vector) {
    this->x = vector.x;
    this->y = vector.y;
    return *this;
}
