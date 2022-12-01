#pragma once
#include <boost/json.hpp>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "game_session.h"

namespace model {

class Game {
 public:
  using Maps = std::vector<Map>;

  void AddMap(Map map);

  // void AddGameSession(GameSession session);

  const Maps& GetMaps() const noexcept { return maps_; }

  const Map* FindMap(const Map::Id& id) const noexcept {
    if (auto it = map_id_to_index_.find(id); it != map_id_to_index_.end()) {
      return &maps_.at(it->second);
    }
    return nullptr;
  }

  void SetDefaultSpeed(double speed) { m_default_speed = speed; }
  double GetDefaultSpeed() const { return m_default_speed; }

  // get exist or create new & return pointer
  std::shared_ptr<GameSession> GetSession(const model::Map::Id& id);

 private:
  using MapIdHasher = util::TaggedHasher<Map::Id>;
  using MapIdToIndex = std::unordered_map<Map::Id, size_t, MapIdHasher>;

  std::vector<Map> maps_;
  MapIdToIndex map_id_to_index_;

  std::vector<std::shared_ptr<GameSession>> m_sess;

  double m_default_speed = 1.0;
};

}  // namespace model
