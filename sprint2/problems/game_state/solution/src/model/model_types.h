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
};

using Vector2i = Vector2<int>;
using Vector2d = Vector2<double>;

}  // namespace model

#endif  // __DIRECTION_VECTOR_H__