#include "game.h"

#include <stdexcept>
#include <string>
namespace model {
using namespace std::string_literals;

void Game::AddMap(Map map) {
  // if dog speed is not set on the map
  if(map.GetDogSpeed() == 0.0) {
    map.SetDogSpeed(GetDefaultSpeed());
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

std::shared_ptr<GameSession> Game::GetSession(const model::Map::Id& id) {
  const Map* map = FindMap(id);
  if (!map) {
    throw std::invalid_argument("Map"s + *id + "id not exist"s);
  }

  for (auto& it : m_sess) {
    if (it->GetMap().GetId() == id) {
      return it;
    }
  }

  auto sess = std::make_shared<GameSession>(*map);
  m_sess.push_back(sess);

   return sess;
  }

}  // namespace model
