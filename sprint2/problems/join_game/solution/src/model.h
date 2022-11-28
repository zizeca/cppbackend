#pragma once
#include <boost/json.hpp>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "model_gses.h"
#include "model_map.h"
#include "model_token.h"
#include "tagged.h"

namespace model {

class Game {
 public:
  using Maps = std::vector<Map>;

  void AddMap(Map map);

  void AddGameSession(GameSession session);

  const Maps& GetMaps() const noexcept { return maps_; }

  const Map* FindMap(const Map::Id& id) const noexcept {
    if (auto it = map_id_to_index_.find(id); it != map_id_to_index_.end()) {
      return &maps_.at(it->second);
    }
    return nullptr;
  }

  /**
   * @brief Get the Session object for existing map
   *
   * @param id model::Map id
   * @return GameSession* if exist or nullptr
   */
  GameSession* GetSession(const model::Map::Id& id) {
    if (m_sessions.contains(id)) {
      return &m_sessions.at(id);
    }
    return nullptr;
  }

 private:
  using MapIdHasher = util::TaggedHasher<Map::Id>;
  using MapIdToIndex = std::unordered_map<Map::Id, size_t, MapIdHasher>;

  std::vector<Map> maps_;
  MapIdToIndex map_id_to_index_;

  // first -> map name or map id
  // second -> array of GameSession on this map
  std::unordered_map<Map::Id, GameSession, util::TaggedHasher<Map::Id>> m_sessions;
};

}  // namespace model
