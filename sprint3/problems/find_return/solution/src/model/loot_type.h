#pragma once
#ifndef __MODEL__LOOT_H__
#define __MODEL__LOOT_H__

#include <string>

#include "geometry.h"

namespace model {

struct LootType {
  std::optional<std::string> name;   // example -> = "key";
  std::optional<std::string> file;   // example -> = "assets/key.obj";
  std::optional<std::string> type;   // example -> = "obj";
  std::optional<int> rotation;       // example -> = 90;
  std::optional<std::string> color;  // example -> = "#338844";
  std::optional<double> scale;       // example -> = 0.03;
  std::optional<size_t> value;
};

}  // namespace model

#endif  // __MODEL__LOOT_H__