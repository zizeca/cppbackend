#include "loot_type.h"

namespace model {

LootType::LootType(const std::string& name,
                   const std::string& file,
                   const std::string& type,
                   const int& rotation,
                   const std::string& color,
                   const double& scale) : m_name(name),
                                          m_file(file),
                                          m_type(type),
                                          m_rotation(rotation),
                                          m_color(color),
                                          m_scale(scale) {
}

const std::string& LootType::GetName() const noexcept {
  return m_name;
}

const std::string& LootType::GetFile() const noexcept {
  return m_file;
}

const std::string& LootType::GetType() const noexcept {
  return m_type;
}

const int& LootType::GetRotation() const noexcept {
  return m_rotation;
}

const std::string& LootType::GetColor() const noexcept {
  return m_color;
}

const double& LootType::GetScale() const noexcept {
  return m_scale;
}

}  // namespace model
