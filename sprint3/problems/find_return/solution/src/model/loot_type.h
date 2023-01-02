#ifndef __LOOT_TYPE_H__
#define __LOOT_TYPE_H__

#include <compare>

namespace model
{
  struct LootType {
  std::optional<std::string> name;   // example -> = "key";
  std::optional<std::string> file;   // example -> = "assets/key.obj";
  std::optional<std::string> type;   // example -> = "obj";
  std::optional<int> rotation;       // example -> = 90;
  std::optional<std::string> color;  // example -> = "#338844";
  std::optional<double> scale;       // example -> = 0.03;
  std::optional<size_t> value;

  auto operator<=>(const LootType&) const = default;

};

} // namespace model


#endif // __LOOT_TYPE_H__