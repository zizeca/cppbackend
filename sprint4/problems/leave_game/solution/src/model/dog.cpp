#include "dog.h"

namespace model {

Dog::Dog(const Dog::Id& id)
    : GameObject({0.0, 0.0}, 0.6),
      m_token(""),
      m_id(id),
      m_speed(0.f, 0.f),
      m_dir("U"s),
      m_default_speed(0.0),
      m_bag_size(0),
      m_score(0),
      m_clock(){

        m_start_idle = m_clock.now();
        m_start_play = m_clock.now();
}

const Dog::Id& Dog::GetId() const {
  return m_id;
}

void Dog::SetToken(const Token& token) {
  m_token = token;
}


const Token& Dog::GetToken() const {
  return m_token;
}

const Point2d& Dog::GetSpeed() const noexcept {
  return m_speed;
}

void Dog::Stop() {
  m_speed.x = 0.0;
  m_speed.y = 0.0;
}

void Dog::SetDefaultSpeed(const double& speed) {
  m_default_speed = speed;
}

const std::string& Dog::GetDir() const noexcept {
  return m_dir;
}

bool model::Dog::IsFull() const {
  return m_bag_size == m_loots.size();
}

void Dog::AddLoot(const Loot& loot) {
  if (m_loots.size() >= m_bag_size) {
    throw std::logic_error("Bag is full");
  }
  m_loots.push_back(loot);
}

std::list<Loot> Dog::UnloadLoots() {
  return std::move(m_loots);
}

const std::list<Loot>& Dog::GetLoots() const noexcept {
  return m_loots;
}

void Dog::AddPoints(int points) {
  m_score += points;
}

int Dog::GetScore() const noexcept {
  return m_score;
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
    ResetIdleTimer();
    return;
  } else {
    throw std::invalid_argument("Argumen\'"s + dir + "\' must be 'U', 'R', 'D', 'L' or \"\"."s);
  }
  m_dir = dir;
}

void Dog::SetBagSize(size_t size) {
  m_bag_size = size;
}

size_t Dog::GetBagSize() const noexcept{
  return m_bag_size;
}

void Dog::UpdateTimer() {
  if(m_speed.x != .0 || m_speed.y != .0) {
    ResetIdleTimer();
  }
}

void Dog::ResetIdleTimer() {
  m_start_idle = m_clock.now();
}


double Dog::GetDownTime() const {
  auto current = m_clock.now();
  auto deltaTime = std::chrono::duration<double>(current - m_start_idle).count();

  return deltaTime;
}

double Dog::GetPlayTime() const {
  auto current = m_clock.now();
  auto deltaTime = std::chrono::duration<double>(current - m_start_play).count();
  return deltaTime;
}


}  // namespace model