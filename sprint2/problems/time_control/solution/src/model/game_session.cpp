#include "game_session.h"
#include "../logger.h"
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
  for (auto it = m_dogs.begin(); it != m_dogs.end(); /* iteration below */) {
    // if dog not more active, or owner not active any more
    if (it->expired()) {
      it = m_dogs.erase(it);
      continue;
    }
    auto dog = it->lock();
    auto dir = dog->GetDir();
    if (dir == "") {
      dog->SetSpeed({0.0, 0.0});
      return;
    }

    auto pos = dog->GetPosition();
    auto speed = dog->GetSpeed();
    auto posNew = pos + (speed * delta);

    auto ver = m_map.GetRoadVerByPos(pos);
    auto hor = m_map.GetRoadHorByPos(pos);

    assert(ver != std::nullopt || hor != std::nullopt);

    auto up = ver ? (static_cast<double>(ver->GetStart().y) - 0.4) : (static_cast<double>(hor->GetStart().y) - 0.4);
    auto lf = hor ? (static_cast<double>(hor->GetStart().x) - 0.4) : (static_cast<double>(ver->GetStart().x) - 0.4);
    auto dw = ver ? (static_cast<double>(ver->GetEnd().y) + 0.4) : (static_cast<double>(hor->GetEnd().y) + 0.4);
    auto rg = hor ? (static_cast<double>(hor->GetEnd().x) + 0.4) : (static_cast<double>(ver->GetEnd().x) + 0.4);

    if (posNew.y <= up) {
      posNew.y = up;
      dir = "";
    } else if (posNew.y >= dw) {
      posNew.y = dw;
      dir = "";
    } else if (posNew.x <= lf) {
      posNew.x = lf;
      dir = "";
    } else if (posNew.x >= rg) {
      posNew.x = rg;
      dir = "";
    }

    Logger::LogDebug("dog pos ("s + std::to_string(posNew.x) + ", "s + std::to_string(posNew.y) + ")"s, "game session");
    dog->SetPosition(posNew);
    dog->SetDir(dir);
  }
}

}  // namespace model
