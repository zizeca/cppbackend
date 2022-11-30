#ifndef __DIRECTION_VECTOR_H__
#define __DIRECTION_VECTOR_H__

template <typename T>
struct Vector2 {
  T x;
  T y;
};

// typedef Vector2<int> Vector2i;
// typedef Vector2<double> Vector2d;

using Vector2i = Vector2<int>;
using Vector2d = Vector2<double>;

#endif // __DIRECTION_VECTOR_H__