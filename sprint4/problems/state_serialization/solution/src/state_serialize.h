#ifndef __STATE_SERIALIZE_H__
#define __STATE_SERIALIZE_H__

#include <boost/serialization/vector.hpp>

#include "model.h"

namespace model
{

template <typename Archive>
void serialize(Archive& ar, Point2d& point, [[maybe_unused]] const unsigned version) {
    ar& point.x;
    ar& point.y;
}

} // namespace model


#endif // __STATE_SERIALIZE_H__