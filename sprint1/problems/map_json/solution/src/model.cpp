#include "model.h"

#include <stdexcept>

namespace model {
using namespace std::literals;

void Map::AddOffice(Office office) {
    if (warehouse_id_to_index_.contains(office.GetId())) {
        throw std::invalid_argument("Duplicate warehouse");
    }

    const size_t index = offices_.size();
    Office& o = offices_.emplace_back(std::move(office));
    try {
        warehouse_id_to_index_.emplace(o.GetId(), index);
    } catch (...) {
        // Удаляем офис из вектора, если не удалось вставить в unordered_map
        offices_.pop_back();
        throw;
    }
}

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

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Building const& building) {
  jv = {{"x", std::to_string(building.GetBounds().position.x)},
        {"y", std::to_string(building.GetBounds().position.y)},
        {"w", std::to_string(building.GetBounds().size.width)},
        {"x", std::to_string(building.GetBounds().size.height)}};
}
/**/
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Office const& office) {
  jv = {{"id", *office.GetId()},
        {"x", std::to_string(office.GetPosition().x)},
        {"y", std::to_string(office.GetPosition().y)},
        {"offsetX", std::to_string(office.GetOffset().dx)},
        {"offsetY", std::to_string(office.GetOffset().dy)}};
}

/**/
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Road const& road) {
  boost::json::object obj;
  obj["x0"] = std::to_string(road.GetStart().x);
  obj["y0"] = std::to_string(road.GetStart().y);
  if (road.IsHorizontal()) {
    obj["x1"] = std::to_string(road.GetEnd().x);
  } else {
    obj["y1"] = std::to_string(road.GetEnd().y);
  }

  jv = boost::json::value_from(obj);
}

/**/
// костыли,
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Map const& map) {
  boost::json::object obj;
  obj["id"] = *map.GetId();
  obj["name"] = map.GetName();

  // roads brief method not working :(
  if (!map.GetRoads().empty()) {
    boost::json::array arr;
    for (auto& i : map.GetRoads()) {
      // auto v = boost::json::value_from(r);
      // arr.push_back(boost::json::value_from(i));
      boost::json::object vobj;
      vobj["x0"] = std::to_string(i.GetStart().x);
      vobj["y0"] = std::to_string(i.GetStart().y);
      if (i.IsHorizontal()) {
        vobj["x1"] = std::to_string(i.GetEnd().x);
      } else {
        vobj["y1"] = std::to_string(i.GetEnd().y);
      }
      arr.push_back(boost::json::value_from(vobj));
    }
    obj["roads"] = boost::json::value_from(arr);
  }

  if (!map.GetBuildings().empty()) {
    boost::json::array arr;
    for (auto& i : map.GetBuildings()) {
      boost::json::value v;
      v = {{"x", std::to_string(i.GetBounds().position.x)},
           {"y", std::to_string(i.GetBounds().position.y)},
           {"w", std::to_string(i.GetBounds().size.width)},
           {"h", std::to_string(i.GetBounds().size.height)}};
      arr.push_back(v);
    }
    obj["buildings"] = boost::json::value_from(arr);
  }

  if (!map.GetOffices().empty()) {
    boost::json::array arr;
    for (auto& office : map.GetOffices()) {
      boost::json::value v;
      v = {{"id", *office.GetId()},
           {"x", std::to_string(office.GetPosition().x)},
           {"y", std::to_string(office.GetPosition().y)},
           {"offsetX", std::to_string(office.GetOffset().dx)},
           {"offsetY", std::to_string(office.GetOffset().dy)}};
      arr.push_back(v);
    }
    obj["offices"] = boost::json::value_from(arr);
  }
  jv = obj;
}

}  // namespace model
