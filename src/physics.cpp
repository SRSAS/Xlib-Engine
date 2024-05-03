#include "../include/physics.h"

using namespace physics;

Force2D Force2D::operator+(const Force2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Force2D Force2D::operator-(const Force2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

template <typename number> Force2D Force2D::operator*(const number &vector) {
  this->x *= vector.x;
  this->y *= vector.y;
  return *this;
}

template <typename number> Force2D Force2D::operator/(const number &vector) {
  this->x /= vector.x;
  this->y /= vector.y;
  return *this;
}

Force2D &Force2D::operator+=(const Force2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Force2D &Force2D::operator-=(const Force2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

Force2D &Force2D::operator*=(const Force2D &vector) {
  this->x *= vector.x;
  this->y *= vector.y;
  return *this;
}

Force2D &Force2D::operator/=(const Force2D &vector) {
  this->x /= vector.x;
  this->y /= vector.y;
  return *this;
}

Acceleration2D Acceleration2D::operator+(const Acceleration2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Acceleration2D Acceleration2D::operator-(const Acceleration2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

template <typename number>
Acceleration2D Acceleration2D::operator*(const number &vector) {
  this->x *= vector.x;
  this->y *= vector.y;
  return *this;
}

template <typename number>
Acceleration2D Acceleration2D::operator/(const number &vector) {
  this->x /= vector.x;
  this->y /= vector.y;
  return *this;
}

Acceleration2D &Acceleration2D::operator+=(const Acceleration2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Acceleration2D &Acceleration2D::operator-=(const Acceleration2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

Acceleration2D &Acceleration2D::operator*=(const Acceleration2D &vector) {
  this->x *= vector.x;
  this->y *= vector.y;
  return *this;
}

Acceleration2D &Acceleration2D::operator/=(const Acceleration2D &vector) {
  this->x /= vector.x;
  this->y /= vector.y;
  return *this;
}

Speed2D &Speed2D::operator/=(const Speed2D &vector) {
  this->x /= vector.x;
  this->y /= vector.y;
  return *this;
}

Speed2D Speed2D::operator+(const Speed2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Speed2D Speed2D::operator-(const Speed2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

template <typename number> Speed2D Speed2D::operator*(const number &vector) {
  this->x *= vector.x;
  this->y *= vector.y;
  return *this;
}

template <typename number> Speed2D Speed2D::operator/(const number &vector) {
  this->x /= vector.x;
  this->y /= vector.y;
  return *this;
}

Speed2D &Speed2D::operator+=(const Speed2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Speed2D &Speed2D::operator-=(const Speed2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

Speed2D &Speed2D::operator*=(const Speed2D &vector) {
  this->x *= vector.x;
  this->y *= vector.y;
  return *this;
}

Position2D &Position2D::operator*=(const Position2D &vector) {
  this->x *= vector.x;
  this->y *= vector.y;
  return *this;
}

Position2D &Position2D::operator/=(const Position2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Position2D Position2D::operator+(const Position2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Position2D Position2D::operator-(const Position2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

template <typename number>
Position2D Position2D::operator*(const number &vector) {
  this->x *= vector.x;
  this->y *= vector.y;
  return *this;
}

template <typename number>
Position2D Position2D::operator/(const number &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Position2D &Position2D::operator+=(const Position2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

Position2D &Position2D::operator-=(const Position2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}
