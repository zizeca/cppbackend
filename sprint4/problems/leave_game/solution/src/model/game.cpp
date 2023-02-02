#include "game.h"

#include <stdexcept>
#include <string>
namespace model {
using namespace std::string_literals;

void Game::AddMap(Map map) {
  // if dog speed is not set on the map
  if (map.GetDogSpeed() == 0.0) {
    map.SetDogSpeed(GetDefaultSpeed());
  }

  // if bag size not set, set default size from game
  if (map.GetBagCapacity() == 0) {
    map.SetBagCapacity(GetDefaultBagCapacity());
  }

  const size_t index = maps_.size();
  if (auto [it, inserted] = map_id_to_index_.emplace(map.GetId(), index); !inserted) {
    throw std::invalid_argument("Map with id "s + *map.GetId() + " already exists"s);
  } else {
    try {
      maps_.emplace_back(std::move(map));
    } catch (...) {
      map_id_to_index_.erase(it);
      throw;
    }
  }
}

Game::SessPtr Game::GetSession(const model::Map::Id& id) {
  auto map = FindMap(id);
  if (!map) {
    throw std::invalid_argument("Map"s + *id + "id not exist"s);
  }

  // if session exist? find and return
  for (const auto& sess : m_sess) {
    if (sess->GetMap().GetId() == id) {
      return sess;
    }
  }

  std::chrono::milliseconds ms(static_cast<int>(m_period_loot_gen * 1000));

  // creat ne session if fail to find exist
  auto sess = std::make_shared<GameSession>(*map, LootGenerator(ms, m_probability_loot_gen));
  sess->SetDogRandomSpawn(m_random_dog_spawn);
  sess->SetRetirementTime(m_dog_retirement_time);
  m_sess.push_back(sess);

  return sess;
}

void Game::Update(double delta_time) {
  for (const auto& sess : m_sess) {
    sess->Update(delta_time);
  }
}

void Game::SetRandomSpawn(bool enable) {
  m_random_dog_spawn = enable;
}

bool Game::IsRandomSpawn() const {
  return m_random_dog_spawn;
}

void Game::SetLootGeneratorConfig(double period, double probability) {
  m_period_loot_gen = period;
  m_probability_loot_gen = probability;
}

void Game::SetDefaultBagCapacity(size_t size) {
  m_default_bag_capacity = size;
}

size_t Game::GetDefaultBagCapacity() const noexcept {
  return m_default_bag_capacity;
}

// void Game::AddSession(std::shared_ptr<GameSession> sess) {
//   m_sess.emplace_back(sess);
// }

void Game::SetDogRetirementTime(double time_in_seconds) {
  m_dog_retirement_time = time_in_seconds;
}

const Game::SessPtrList& Game::GetSessionList() const {
  return m_sess;
}

}  // namespace model
