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

/* 
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Building const& building) {
  jv = {{"x", std::to_string(building.GetBounds().position.x)},
        {"y", std::to_string(building.GetBounds().position.y)},
        {"w", std::to_string(building.GetBounds().size.width)},
        {"x", std::to_string(building.GetBounds().size.height)}};
}

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Office const& office) {
  jv = {{"id", *office.GetId()},
        {"x", std::to_string(office.GetPosition().x)},
        {"y", std::to_string(office.GetPosition().y)},
        {"offsetX", std::to_string(office.GetOffset().dx)},
        {"offsetY", std::to_string(office.GetOffset().dy)}};
}


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


void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Map const& map) {
  boost::json::object obj;
  obj["id"] = *map.GetId();
  obj["name"] = map.GetName();

  // road parse
  if (!map.GetRoads().empty()) {
    boost::json::array arr;
    for (auto& i : map.GetRoads()) {
      arr.push_back(boost::json::value_from(i));
    }
    obj["roads"] = boost::json::value_from(arr);
  }

  // building parse
  if (!map.GetBuildings().empty()) {
    boost::json::array arr;
    for (auto& i : map.GetBuildings()) {
      arr.push_back(boost::json::value_from(i));
    }
    obj["buildings"] = boost::json::value_from(arr);
  }

  // office parse
  if (!map.GetOffices().empty()) {
    boost::json::array arr;
    for (auto& office : map.GetOffices()) {
      arr.push_back(boost::json::value_from(office));
    }
    obj["offices"] = boost::json::value_from(arr);
  }
  jv = obj;
}

Building tag_invoke(boost::json::value_to_tag<Building>, boost::json::value& jv){

}

Office tag_invoke(boost::json::value_to_tag<Office>, boost::json::value& jv){

}

Road tag_invoke(boost::json::value_to_tag<Road>, boost::json::value& jv){

}

Map tag_invoke(boost::json::value_to_tag<Map>, boost::json::value& jv){

}

 */
}  // namespace model
