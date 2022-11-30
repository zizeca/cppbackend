#pragma once
#include <boost/json.hpp>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "model_gses.h"

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

  void SetDefaultSpeed(Vector2d speed) { m_default_speed = speed; }
  Vector2d GetDefaultSpeed() const { return m_default_speed; }

  /**
   * @brief Get the Session object for existing map
   *
   * @param id model::Map id
   * @return GameSession&
   */
  GameSession& GetSession(const model::Map::Id& id) {
    if (m_sessions.contains(id)) {
      return m_sessions.at(id);
    }

    const Map* map = FindMap(id);
    if (!map) {
      throw std::invalid_argument("Map"s + *id + "id not exist"s);
    }

    auto p = m_sessions.emplace(id, *map);
    if (!p.second) {
      throw std::logic_error("Fail to emplace map"s + *id);
    }

    return p.first->second;
  }

 private:
  using MapIdHasher = util::TaggedHasher<Map::Id>;
  using MapIdToIndex = std::unordered_map<Map::Id, size_t, MapIdHasher>;

  std::vector<Map> maps_;
  MapIdToIndex map_id_to_index_;

  // first -> map name or map id
  // second -> array of GameSession on this map
  std::unordered_map<Map::Id, GameSession, util::TaggedHasher<Map::Id>> m_sessions;

  Vector2d m_default_speed = Vector2d{1.0, 1.0};
};

}  // namespace model