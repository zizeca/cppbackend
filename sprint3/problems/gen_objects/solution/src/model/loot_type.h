#pragma once
#ifndef __MODEL__LOOT_TYPE_H__
#define __MODEL__LOOT_TYPE_H__

#include <string>

namespace model {

class LootType {
 public:
  LootType(const std::string& name, const std::string& file, const std::string& type, const int& rotation, const std::string& color, const double& scale);

  const std::string& GetName() const noexcept;

  const std::string& GetFile() const noexcept;

  const std::string& GetType() const noexcept;

  const int& GetRotation() const noexcept;

  const std::string& GetColor() const noexcept;

  const double& GetScale() const noexcept;

 private:
  std::string m_name;   // example -> = "key";
  std::string m_file;   // example -> = "assets/key.obj";
  std::string m_type;   // example -> = "obj";
  int m_rotation;       // example -> = 90;
  std::string m_color;  // example -> = "#338844";
  double m_scale;       // example -> = 0.03;
};

}  // namespace model

#endif  // __MODEL__LOOT_TYPE_H__