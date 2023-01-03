#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "geometry.h"

namespace model {

class GameObject {
 public:
  void SetPosition(Point2d const& position);
  const Point2d& GetPosition() const noexcept;

  void SetWidth(double const& width);
  const double& GetWidth() const noexcept;

 protected:
  GameObject(Point2d const& pos = Point2d(), double const& width = 0.0);
  virtual ~GameObject() = default;
  Point2d m_position;
  double m_width;

  
};

}  // namespace model
#endif  // __GAME_OBJECT_H__