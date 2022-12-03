#include "model.h"

#include <stdexcept>
#include <string>
namespace model {
using namespace std::string_literals;

void Game::AddMap(Map map) {
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
    if (it->GetMapId() == id) {
      return it;
    }
  }

  auto gs = std::make_shared<GameSession>(id);
  m_sess.push_back(gs);
   return gs;
  }

}  // namespace model
