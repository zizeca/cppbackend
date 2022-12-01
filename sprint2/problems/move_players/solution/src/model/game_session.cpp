#include "game_session.h"

namespace model {
GameSession::GameSession(const Map::Id& id) : m_mapId(id) {
}

GameSession::~GameSession() {
}

const Map::Id& GameSession::GetMapId() const noexcept {
  return m_mapId;
}

std::vector<std::shared_ptr<Dog>> GameSession::GetDogs() {
  std::vector<std::shared_ptr<Dog>> ret;
  ret.reserve(m_dogs.size());

  for (auto it = m_dogs.begin(); it != m_dogs.end();) {
    if (it->expired())
      it = m_dogs.erase(it);
    else
      ret.push_back(it->lock());
    ++it;
  }
  return ret;
}

std::shared_ptr<Dog> GameSession::GetDog(const Token& token) {
  for (auto& i : m_dogs) {
    auto ptr = i.lock();
    if (ptr->GetToken() == token) {
      return ptr;
    }
  }
  return nullptr;
}

void GameSession::AddDog(std::shared_ptr<Dog> dog) {
  m_dogs.emplace_back(dog);
}

}  // namespace model
