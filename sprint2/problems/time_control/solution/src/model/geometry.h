#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

namespace gm {

template <typename T>
struct Vector2 {
  Vector2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {}

  template <typename A, typename B>
  Vector2(const A& a, const B& b) : x(static_cast<T>(a)), y(static_cast<T>(b)) {}

  template <typename U>
  Vector2(const Vector2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}
  // field
  T x;
  T y;

  // operators overload
  Vector2& operator+=(const Vector2<T>& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  friend Vector2 operator+(Vector2<T> left, const Vector2<T>& right) {
    left += right;
    return left;
  }

  Vector2& operator+=(const T& scalar) {
    x += scalar;
    y += scalar;
    return *this;
  }

  friend Vector2 operator+(Vector2<T> left, const T& scalar) {
    left += scalar;
    return left;
  }

  friend Vector2 operator+(const T& scalar, Vector2<T> right) {
    right.x = scalar + right.x;
    right.y = scalar + right.y;
    return right;
  }

  Vector2& operator-=(const T& scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
  }

  friend Vector2 operator-(Vector2<T> left, const T& scalar) {
    left -= scalar;
    return left;
  }

  friend Vector2 operator-(const T& scalar, Vector2<T> right) {
    right.x = scalar - right.x;
    right.y = scalar - right.y;
    return right;
  }

  Vector2& operator*=(const T& scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  friend Vector2 operator*(Vector2<T> left, const T& scalar) {
    left *= scalar;
    return left;
  }

  friend Vector2 operator*(const T& scalar, Vector2<T> right) {
    right *= scalar;
    return right;
  }
};

using Vector2i = Vector2<int>;
using Vector2d = Vector2<double>;

template <typename T>
struct Rect {
  Rect(Vector2<T> pos, Vector2<T> size) : left(pos.x), top(pos.y), width(size.x), height(size.y) {}
  Rect(T x, T y, T w, T h) : left(x), top(y), width(w), height(h) {}
  T left;
  T top;
  T width;
  T height;

  bool contains(const Vector2<T>& point) const {
    // Not using 'std::min' and 'std::max' to avoid depending on '<algorithm>'
    const auto min = [](T a, T b) { return (a < b) ? a : b; };
    const auto max = [](T a, T b) { return (a < b) ? b : a; };

    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    // Compute the real min and max of the rectangle on both axes
    const T minX = min(left, static_cast<T>(left + width));
    const T maxX = max(left, static_cast<T>(left + width));
    const T minY = min(top, static_cast<T>(top + height));
    const T maxY = max(top, static_cast<T>(top + height));

    return (point.x >= minX) && (point.x < maxX) && (point.y >= minY) && (point.y < maxY);
  }
};

}  // namespace gm

#endif  // __GEOMETRY_H__