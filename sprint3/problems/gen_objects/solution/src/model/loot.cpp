#include "loot.h"

namespace model {

Loot::Loot(const std::string& name,
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

const std::string& Loot::GetName() const noexcept {
  return m_name;
}

const std::string& Loot::GetFile() const noexcept {
  return m_file;
}

const std::string& Loot::GetType() const noexcept {
  return m_type;
}

const int& Loot::GetRotation() const noexcept {
  return m_rotation;
}

const std::string& Loot::GetColor() const noexcept {
  return m_color;
}

const double& Loot::GetScale() const noexcept {
  return m_scale;
}

}  // namespace model
