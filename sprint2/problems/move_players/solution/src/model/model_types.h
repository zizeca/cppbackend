#ifndef __DIRECTION_VECTOR_H__
#define __DIRECTION_VECTOR_H__

namespace model {

using Dimension = int;
using Coord = Dimension;

struct Point {
  Coord x, y;
};

struct Size {
  Dimension width, height;
};

struct Rectangle {
  Point position;
  Size size;
};

struct Offset {
  Dimension dx, dy;
};

template <typename T>
struct Vector2 {
  Vector2() = default;
  Vector2(const T& X, const T& Y) : x(X), y(Y) {}
  T x;
  T y;

  Vector2& operator+=(const Vector2<T>& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  friend Vector2 operator+(Vector2<T> lhs, const Vector2<T>& rhs) {
    lhs += rhs;
    return lhs;
  }

  Vector2& operator*=(const T& scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  friend Vector2 operator*(Vector2<T> lhs, const T& scalar) {
    lhs *= scalar;
    return lhs;
  }

  friend Vector2 operator*(const T& scalar, Vector2<T> lhs) {
    lhs *= scalar;
    return lhs;
  }
};

using Vector2i = Vector2<int>;
using Vector2d = Vector2<double>;

}  // namespace model

#endif  // __DIRECTION_VECTOR_H__