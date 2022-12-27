#pragma once
#ifndef __MODEL__LOOT_TYPE_H__
#define __MODEL__LOOT_TYPE_H__

#include <string>
#include <optional>

namespace model {

struct LootType {
  std::optional<std::string> name;   // example -> = "key";
  std::optional<std::string> file;   // example -> = "assets/key.obj";
  std::optional<std::string> type;   // example -> = "obj";
  std::optional<int> rotation;       // example -> = 90;
  std::optional<std::string> color;  // example -> = "#338844";
  std::optional<double> scale;       // example -> = 0.03;
  std::optional<int> value;
};

}  // namespace model

#endif  // __MODEL__LOOT_TYPE_H__