#ifndef __ITEM_H__
#define __ITEM_H__

#include "geometry.h"
#include "loot_type.h"

namespace model {

class Item {
 public:
  Item(const LootType& type, const Point2d& pos, const unsigned& loot_index) : m_loot_type(type), m_position(pos), m_loot_index(loot_index) {}

  // void SetPosition(const Point2d& pos);

  const LootType& GetLootType() const noexcept { return m_loot_type; }
  const Point2d& GetPosition() const noexcept { return m_position; }
  const unsigned& GetLootIndex() const noexcept { return m_loot_index; }

 private:
  const LootType m_loot_type;
  const Point2d m_position;
  const unsigned m_loot_index;
};

}  // namespace model

#endif  // __ITEM_H__