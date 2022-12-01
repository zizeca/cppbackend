#include "player.h"

namespace model {

int Player::ID_COUNTER = 0;

Player::Player(const Token& token) : m_id(ID_COUNTER++), m_token(token), m_dog(std::make_shared<Dog>(token)) {
}

int Player::GetId() const noexcept {
  return m_id;
}

const std::string& Player::GetName() const noexcept {
  return m_name;
}

void Player::SetName(const std::string& name) {
  if (name.empty())
    throw std::invalid_argument("name is empty");

  if (!m_name.empty())
    throw std::logic_error("Name already preset");

  m_name = name;
}

std::shared_ptr<Dog> Player::GetDog() {
  return m_dog;
}

void Player::SetDog(std::shared_ptr<Dog> dog) {
  if (!dog)
    throw std::invalid_argument("don is null");

  if (m_dog)
    throw std::logic_error("player already has dog");
  m_dog = dog;
}

const Token& Player::GetToken() const {
  return m_token;
}

std::shared_ptr<GameSession> Player::GetSession() {
  return m_gs;
}

const std::shared_ptr<GameSession> Player::GetSession() const {
  return m_gs;
}

}  // namespace model
