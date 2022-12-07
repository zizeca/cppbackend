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
    // auto ptr = i.lock();
    auto ptr = i;
    if (ptr->GetToken() == token) {
      return ptr;
    }
  }
  return nullptr;
}

void GameSession::AddDog(std::shared_ptr<Dog> dog) {
  // dog->SetPosition(m_map.GetRandPoint());
  dog->SetDefaultSpeed(m_map.GetDogSpeed());
  m_dogs.emplace_back(dog);
}

void GameSession::Update(const double& delta) {
  assert(m_dogs.size() != 0);
  for (auto it = m_dogs.begin(); it != m_dogs.end(); ++it) {

    auto dog = *it;
    assert(dog != nullptr);
    auto dir = dog->GetDir();

    auto pos = dog->GetPosition();
    auto speed = dog->GetSpeed();
    auto posNew = pos + (speed * delta);

    auto ver = m_map.GetRoadVerByPos(pos);
    auto hor = m_map.GetRoadHorByPos(pos);
    // std::abort();
    assert(ver != std::nullopt || hor != std::nullopt);

    auto up = [&ver, &hor](){ return ver ? (static_cast<double>(std::min(ver->GetStart().y, ver->GetEnd().y)) - 0.4) : (static_cast<double>(hor->GetStart().y) - 0.4);};
    auto lf = [&ver, &hor](){ return hor ? (static_cast<double>(std::min(hor->GetStart().x, hor->GetEnd().x)) - 0.4) : (static_cast<double>(ver->GetStart().x) - 0.4);};
    auto dw = [&ver, &hor](){ return ver ? (static_cast<double>(std::max(ver->GetEnd().y, ver->GetStart().y)) + 0.4) : (static_cast<double>(hor->GetEnd().y) + 0.4);};
    auto rg = [&ver, &hor](){ return hor ? (static_cast<double>(std::max(hor->GetEnd().x, hor->GetStart().x)) + 0.4) : (static_cast<double>(ver->GetEnd().x) + 0.4);};

    if (posNew.y < up()) {
      posNew.y = up();
      dog->SetSpeed({0.0,0.0});
    } else if (posNew.y > dw()) {
      posNew.y = dw();
      dog->SetSpeed({0.0,0.0});
    } else if (posNew.x < lf()) {
      posNew.x = lf();
      dog->SetSpeed({0.0,0.0});
    } else if (posNew.x > rg()) {
      posNew.x = rg();
      dog->SetSpeed({0.0,0.0});
    }

    dog->SetPosition(posNew);

    ++it;
  }
}

}  // namespace model
