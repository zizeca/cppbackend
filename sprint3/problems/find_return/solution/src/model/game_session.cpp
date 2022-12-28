#include "game_session.h"

#include <random>

#include "../logger.h"

namespace model {

namespace {

  unsigned int Rand( unsigned max) {
    if (max <= 1) return max;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,max);
    return distribution(generator);
  }

}

GameSession::GameSession(const Map& map, LootGenerator gen) : m_map(map), m_random_spawn(false), m_loot_gen(std::move(gen)) {
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
  GenerateLoot(delta);

  DogsUpdate(delta);
}

void GameSession::SetDogRandomSpawn(bool enable){
  m_random_spawn = enable;
}

void GameSession::GenerateLoot(const double& delta)
{
  const size_t max_loots = m_map.GetLootTypes().size(); 
  if(max_loots == 0 || m_loots.size() >= m_dogs.size()) return;

  const std::chrono::milliseconds ms (static_cast<int64_t>(delta*1000));

  const unsigned count = m_loot_gen.Generate(ms, m_loots.size(), m_dogs.size());

  for(size_t i = 0; i < count; ++i) {
    const unsigned loot_index = Rand(max_loots-1);
    assert(loot_index <= m_map.GetLootTypes().size());
    m_loots.emplace_back(m_map.GetLootTypes().at(loot_index), m_map.GetRandPoint(), loot_index);
  }
}

void GameSession::DogsUpdate(const double& delta)
{
    // dog udate
  for (auto it = m_dogs.begin(); it != m_dogs.end(); ++it) {
    auto dog = *it;
    assert(dog != nullptr);
    auto dir = dog->GetDir();

    auto pos = dog->GetPosition();
    auto speed = dog->GetSpeed();

    auto nextPos = pos + (speed * delta);

    // border define
    Point2d up_left = pos; 
    Point2d down_right = pos;

    // border update
    for( auto road = m_map.GetRoads().cbegin(); road != m_map.GetRoads().cend(); ++road) {
      if(road->Contains(pos)) {
        road->BorderExpansion(up_left, down_right);
      }
    }

    // next position slice if collision detect
    if (nextPos.y < up_left.y) {
      nextPos.y = up_left.y;
      dog->Stop();
    } else if (nextPos.y > down_right.y) {
      nextPos.y = down_right.y;
      dog->Stop();
    } else if (nextPos.x < up_left.x) {
      nextPos.x = up_left.x;
      dog->Stop();
    } else if (nextPos.x > down_right.x) {
      nextPos.x = down_right.x;
      dog->Stop();
    }

    dog->SetPosition(nextPos);
  }
}


}  // namespace model
