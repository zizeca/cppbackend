#pragma once
#ifndef __MODEL__LOOT_H__
#define __MODEL__LOOT_H__

#include <optional>
#include <string>

#include "geometry.h"
#include "loot_type.h"
#include "game_object.h"


namespace model {

class Loot : public GameObject{
 public:
  Loot(const LootType& type,const Point2d& pos, const unsigned& loot_index) : m_loot_type(type), GameObject(pos), m_loot_index(loot_index) {}

  const LootType& GetLootType() const noexcept { return m_loot_type;}
  const unsigned& GetLootIndex() const noexcept {return m_loot_index;}

 private:
  const LootType m_loot_type;
  const unsigned m_loot_index;
};

}  // namespace model

#endif  // __MODEL__LOOT_H__