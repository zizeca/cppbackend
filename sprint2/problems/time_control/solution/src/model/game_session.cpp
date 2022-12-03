#include "game_session.h"

namespace model {
GameSession::GameSession(const Map& map) : m_map(map) {
}

GameSession::~GameSession() {
}

const Map& GameSession::GetMap() const noexcept {
  return m_map;
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
  dog->SetPosition(m_map.GetRandPoint());
  dog->SetDefaultSpeed(m_map.GetDogSpeed());

  m_dogs.emplace_back(dog);
}

void GameSession::Update(const double& delta) {
  for(auto it = m_dogs.begin(); it != m_dogs.end(); /* iteration below */ ) {
    // if dog not more active, or owner not active any more
    if(it->expired()){
      it = m_dogs.erase(it);
      continue;
    }

    auto dog = it->lock();
    auto pos =  dog->GetPosition();

    

    it++;
  }
}

}  // namespace model
