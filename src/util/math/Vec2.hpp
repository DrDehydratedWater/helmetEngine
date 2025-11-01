#pragma once
#include <iostream>

struct Vec2 {
  double x;
  double y;

  Vec2() : x(0.0), y(0.0) {}
  Vec2(double x, double y) : x(x), y(y) {}

  Vec2 operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
  }

  Vec2 operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
  }

  Vec2 &operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;

    return *this;
  }

  Vec2 &operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;

    return *this;
  }

  Vec2 operator*(double scalar) const {
    return Vec2(x * scalar, y * scalar);
  }

  Vec2 operator/(double divisor) const {
    return Vec2(x / divisor, y / divisor);
  }

  Vec2 &operator*=(double scalar) {
    x *= scalar;
    y *= scalar;

    return *this;
  }

  Vec2 &operator/=(double divisor) {
    x /= divisor;
    y /= divisor;

    return *this;
  }


  friend std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
  }
};