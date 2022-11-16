#include "tag_invokers.h"

namespace model {

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



/* model::Building tag_invoke(boost::json::value_to_tag<model::Building>, boost::json::value& jv) {
  if (jv.as_object().size() != 4) {
    throw std::logic_error("rong building val");
  }

  auto x = jv.as_object().at("x").as_int64();
  auto y = jv.as_object().at("y").as_int64();
  auto w = jv.as_object().at("w").as_int64();
  auto h = jv.as_object().at("h").as_int64();

  if (w == 0 || h == 0) {
    throw std::logic_error("rong building size");
  }

  Rectangle rect{{x, y}, {w, h}};

  return Building(rect);
} */
/* 
model::Office tag_invoke(boost::json::value_to_tag<model::Office>, boost::json::value& jv) {
  auto& office = jv.as_object();

  model::Office::Id id(std::string(office.at("id").as_string()));
  model::Point position{office.at("x").as_int64(), office.at("y").as_int64()};
  model::Offset offset{office.at("offsetX").as_int64(), office.at("offsetY").as_int64()};

  return {id, position, offset};
}
 */

/* 
model::Road tag_invoke(boost::json::value_to_tag<model::Road>, boost::json::value& jv) {  // NOLINT
  Point start{};
  Coord finish{};

  auto& road = jv.as_object();
  if (road.size() != 3) {
    throw std::logic_error("wrong argument in road value");
  }

  bool vertical = true;
  for (auto i = road.cbegin(); i != road.cend(); ++i) {
    if (i->key() == "x0") {
      start.x = static_cast<model::Coord>(i->value().as_int64());
    } else if (i->key() == "y0") {
      start.y = static_cast<model::Coord>(i->value().as_int64());
    } else if (i->key() == "x1") {
      vertical = false;
      finish = static_cast<model::Coord>(i->value().as_int64());
    } else if (i->key() == "y1") {
      vertical = true;
      finish = static_cast<model::Coord>(i->value().as_int64());
    } else {
      throw std::logic_error("Unknon key in road json");
    }
  }

  if (vertical) {
    return {Road::VERTICAL, start, finish};
  }
  return {Road::HORIZONTAL, start, finish};
}
 */
/* 
model::Map tag_invoke(boost::json::value_to_tag<model::Map>, boost::json::value& jv) {
  auto& m = jv.as_object();
  Map map(Map::Id(std::string(m.at("id").as_string())), std::string(m.at("name").as_string()));

  for (auto i = m.cbegin(); i != m.cend(); ++i) {
    if (i->key() == "id" || i->key() == "name") {
    } else if (i->key() == "roads") {
      for (auto r : i->value().as_array()) {
        map.AddRoad(boost::json::value_to<Road>(r));
      }
    } else if (i->key() == "buildings") {
      for (auto b : i->value().as_array()) {
        map.GetBuildings(boost::json::value_to<Building>(b));
      }
    } else if (i->key() == "offices") {
      for (auto office : i->value().as_array()) {
        map.AddOffice(boost::json::value_to<Office>(office));
      }
    }
  }
  return map;
}
 */
}  // namespace model