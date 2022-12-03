#include "dog.h"

namespace model {

int Dog::ID_COUNTER = 0;

Dog::Dog(const Token& token) : m_token(token), m_id(ID_COUNTER++), m_pos(0.f, 0.f), m_speed(0.f, 0.f), m_dir("U"s), m_default_speed(0.0) {
}

bool Dog::SetToken(const Token& token) {
  if ((*m_token).empty()) {
    m_token = token;
    return true;
  }
  return false;  // if preset
}

const Vector2d& Dog::GetPosition() const {
  return m_pos;
}

void Dog::SetPosition(const Vector2d& pos) {
  m_pos.x = pos.x;
  m_pos.y = pos.y;
}

const Vector2d& Dog::GetSpeed() const {
  return m_speed;
}

void Dog::SetSpeed(const Vector2d& vspeed) {
  m_speed.x = vspeed.x;
  m_speed.y = vspeed.y;
}

void Dog::SetDefaultSpeed(const double& speed) {
  m_default_speed = speed;
}

const std::string& Dog::GetDir() const {
  return m_dir;
}

void Dog::SetDir(const std::string& dir) {
  if (dir == "U" || dir == "R" || dir == "D" || dir == "L") {
    m_dir = dir;
    return;
  }
  throw std::invalid_argument("Argumen must one of a letters 'U', 'R', 'D', 'L'.");
}

void Dog::Move(const Vector2d& offset) {
  m_pos.x += offset.x;
  m_pos.y += offset.y;
}

Token Dog::GetToken() const {
  return m_token;
}

int Dog::GetId() const {
  return m_id;
}

}  // namespace model