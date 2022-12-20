#include "game_session.h"

#include "../logger.h"
namespace model {
GameSession::GameSession(const Map& map) : m_map(map), m_random_spawn(false) {
}

GameSession::~GameSession() {
}

const Map& GameSession::GetMap() const noexcept {
  return m_map;
}

std::shared_ptr<Dog> GameSession::GetDog(const Token& token) {
  for (auto& i : m_dogs) {
    auto ptr = i;
    if (ptr->GetToken() == token) {
      return ptr;
    }
  }
  return nullptr;
}

void GameSession::AddDog(std::shared_ptr<Dog> dog) {
  dog->SetPosition(m_map.GetRandPoint(m_random_spawn));
  dog->SetDefaultSpeed(m_map.GetDogSpeed());
  m_dogs.emplace_back(dog);
}

void GameSession::Update(const double& delta) {
  for (auto it = m_dogs.begin(); it != m_dogs.end(); ++it) {
    auto dog = *it;
    assert(dog != nullptr);
    auto dir = dog->GetDir();

    auto pos = dog->GetPosition();
    auto speed = dog->GetSpeed();

    auto posNew = pos + (speed * delta);

    double border_up = pos.y;
    double border_down = pos.y;
    double border_left = pos.x;
    double border_right = pos.x;

     for( auto road = m_map.GetRoads().cbegin(); road != m_map.GetRoads().cend(); ++road) {
      if(road->Contains(pos)) {
        if(border_up > road->GetMinY()) {
          border_up = road->GetMinY();
        }

        if(border_down < road->GetMaxY()) {
          border_down = road->GetMaxY();
        }

        if(border_left > road->GetMinX()) {
          border_left = road->GetMinX();
        }

        if(border_right < road->GetMaxX()) {
          border_right = road->GetMaxX();
        }
      }
    }

    if (posNew.y < border_up) {
      posNew.y = border_up;
      dog->Stop();
    } else if (posNew.y > border_down) {
      posNew.y = border_down;
      dog->Stop();
    } else if (posNew.x < border_left) {
      posNew.x = border_left;
      dog->Stop();
    } else if (posNew.x > border_right) {
      posNew.x = border_right;
      dog->Stop();
    }

    dog->SetPosition(posNew);
  }
}

void GameSession::SetDogRandomSpawn(bool enable){
  m_random_spawn = enable;
}


}  // namespace model
