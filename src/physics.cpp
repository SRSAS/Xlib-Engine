#include "../include/physics.h"

physics::Force2D physics::Force2D::operator+(const physics::Force2D &vector) {
  physics::Force2D temp(x + vector.x, y + vector.y);
  return temp;
}

physics::Force2D physics::Force2D::operator-(const physics::Force2D &vector) {
  physics::Force2D temp(x - vector.x, y - vector.y);
  return temp;
}

physics::Force2D physics::Force2D::operator*(const double &scalar) const {
  physics::Force2D temp(x * scalar, y * scalar);
  return temp;
}

physics::Force2D physics::Force2D::operator/(const double &scalar) const {
  physics::Force2D temp(x / static_cast<double>(scalar),
                        y / static_cast<double>(scalar));
  return temp;
}

physics::Force2D &physics::Force2D::operator+=(const physics::Force2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

physics::Force2D &physics::Force2D::operator-=(const physics::Force2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

physics::Force2D &physics::Force2D::operator*=(const double &scalar) {
  this->x *= scalar;
  this->y *= scalar;
  return *this;
}

physics::Force2D &physics::Force2D::operator/=(const double &scalar) {
  this->x /= scalar;
  this->y /= scalar;
  return *this;
}

physics::Acceleration2D
physics::Acceleration2D::operator+(const physics::Acceleration2D &vector) {
  physics::Acceleration2D temp(x + vector.x, y + vector.y);
  return temp;
}

physics::Acceleration2D
physics::Acceleration2D::operator-(const physics::Acceleration2D &vector) {
  physics::Acceleration2D temp(x - vector.x, y - vector.y);
  return temp;
}

physics::Acceleration2D
physics::Acceleration2D::operator*(const double &scalar) {
  physics::Acceleration2D temp(x * scalar, y * scalar);
  return temp;
}

physics::Acceleration2D
physics::Acceleration2D::operator/(const double &scalar) {
  physics::Acceleration2D temp(x / scalar, y / scalar);
  return temp;
}

physics::Acceleration2D &
physics::Acceleration2D::operator+=(const physics::Acceleration2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

physics::Acceleration2D &
physics::Acceleration2D::operator-=(const physics::Acceleration2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

physics::Acceleration2D &
physics::Acceleration2D::operator*=(const double &scalar) {
  this->x *= scalar;
  this->y *= scalar;
  return *this;
}

physics::Acceleration2D &
physics::Acceleration2D::operator/=(const double &scalar) {
  this->x /= scalar;
  this->y /= scalar;
  return *this;
}

physics::Speed2D physics::Speed2D::operator+(const physics::Speed2D &vector) {
  physics::Speed2D temp(x + vector.x, y + vector.y);
  return temp;
}

physics::Speed2D physics::Speed2D::operator-(const physics::Speed2D &vector) {
  physics::Speed2D temp(x - vector.x, y - vector.y);
  return temp;
}

physics::Speed2D physics::Speed2D::operator*(const double &scalar) {
  physics::Speed2D temp(x * scalar, y * scalar);
  return temp;
}

physics::Speed2D physics::Speed2D::operator/(const double &scalar) {
  physics::Speed2D temp(x / scalar, y / scalar);
  return temp;
}

physics::Speed2D &physics::Speed2D::operator+=(const physics::Speed2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

physics::Speed2D &physics::Speed2D::operator-=(const physics::Speed2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

physics::Speed2D &physics::Speed2D::operator*=(const double &scalar) {
  this->x *= scalar;
  this->y *= scalar;
  return *this;
}

physics::Speed2D &physics::Speed2D::operator/=(const double &scalar) {
  this->x /= scalar;
  this->y /= scalar;
  return *this;
}

physics::Position2D
physics::Position2D::operator+(const physics::Position2D &vector) {
  physics::Position2D temp(x + vector.x, y + vector.y);
  return temp;
}

physics::Position2D
physics::Position2D::operator-(const physics::Position2D &vector) {
  physics::Position2D temp(x - vector.x, y - vector.y);
  return temp;
}

physics::Position2D physics::Position2D::operator*(const double &scalar) {
  physics::Position2D temp(x * scalar, y * scalar);
  return temp;
}

physics::Position2D physics::Position2D::operator/(const double &scalar) {
  physics::Position2D temp(x / scalar, y / scalar);
  return temp;
}

physics::Position2D &
physics::Position2D::operator+=(const physics::Position2D &vector) {
  this->x += vector.x;
  this->y += vector.y;
  return *this;
}

physics::Position2D &
physics::Position2D::operator-=(const physics::Position2D &vector) {
  this->x -= vector.x;
  this->y -= vector.y;
  return *this;
}

physics::Position2D &physics::Position2D::operator*=(const double &scalar) {
  this->x *= scalar;
  this->y *= scalar;
  return *this;
}

physics::Position2D &physics::Position2D::operator/=(const double &scalar) {
  this->x /= scalar;
  this->y /= scalar;
  return *this;
}
