#ifndef __POINT_SERIALIZATION_H__
#define __POINT_SERIALIZATION_H__

#include "../model.h"

namespace model
{
template <typename Archive>
void serialize(Archive& ar, Point2d& point, [[maybe_unused]] const unsigned version) {
  ar& point.x;
  ar& point.y;
}

} // namespace model


#endif // __POINT_SERIALIZATION_H__