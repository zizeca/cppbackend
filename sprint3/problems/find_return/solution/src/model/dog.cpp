#include "dog.h"

namespace model {

int Dog::ID_COUNTER = 0;

Dog::Dog(const Token& token) : m_token(token), m_id(ID_COUNTER++), m_speed(0.f, 0.f), m_dir("U"s), m_default_speed(0.0) {
}

bool Dog::SetToken(const Token& token) {
  if ((*m_token).empty()) {
    m_token = token;
    return true;
  }
  return false;  // if preset
}

const Point2d& Dog::GetSpeed() const {
  return m_speed;
}

void Dog::Stop() {
  m_speed.x = 0.0;
  m_speed.y = 0.0;
}

void Dog::SetDefaultSpeed(const double& speed) {
  m_default_speed = speed;
}

const std::string& Dog::GetDir() const {
  return m_dir;
}

void Dog::SetDir(const std::string& dir) {
  if (dir == "U") {
    m_speed = {0.0, -m_default_speed};
  } else if (dir == "R") {
    m_speed = {m_default_speed, 0.0};
  } else if (dir == "D") {
    m_speed = {0.0, m_default_speed};
  } else if (dir == "L") {
    m_speed = {-m_default_speed, 0.0};
  } else if (dir == "") {
    m_speed = {0.0, 0.0};
    return;
  } else {
    throw std::invalid_argument("Argumen\'"s + dir + "\' must be 'U', 'R', 'D', 'L' or \"\"."s);
  }
  m_dir = dir;
}

// void Dog::Move(const Point2d& offset) {
//   m_pos.x += offset.x;
//   m_pos.y += offset.y;
// }

Token Dog::GetToken() const {
  return m_token;
}

int Dog::GetId() const {
  return m_id;
}

// double Dog::GetDefaultSpeed() const {
//   return m_default_speed;
// }

}  // namespace model