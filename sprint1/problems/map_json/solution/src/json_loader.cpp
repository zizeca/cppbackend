#include "json_loader.h"

#define BOOST_JSON_STANDALONE
#include <boost/json.hpp>
#include <boost/json/system_error.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "boost_json.cpp"

using namespace std::literals;

/**  json map
{
  "maps": [
    {
      "id": "map1",
      "name": "Map 1",
      "roads": [
        {"x0": 0,  "y0": 0,  "x1": 40},
        {"x0": 40, "y0": 0,  "y1": 30},
        {"x0": 40, "y0": 30, "x1": 0 },
        {"x0": 0,  "y0": 0,  "y1": 30}
      ],
      "buildings": [
        {"x": 5, "y": 5, "w": 30, "h": 20}
      ],
      "offices": [
        {"id": "o0", "x": 40, "y": 30, "offsetX": 5, "offsetY": 0}
      ]
    }
  ]
}
*/

namespace {
void parseRoads(const boost::json::array& arr, model::Map& map) {
  if (arr.size() < 1) return;  // The array must contain at least one element
  try {
    for (auto i : arr) {
      model::Point start{0, 0};
      // model::Coord end_x{0}, end_y{0};

      model::Coord end{0};

      bool vertical = true;

      auto r = i.as_object();
      if (r.size() != 3) {
        throw std::logic_error("Too rong cound fields in road");
      }

      for (auto k = r.begin(); k != r.end(); ++k) {
        if (k->key() == "x0") {
          start.x = static_cast<model::Coord>(k->value().as_int64());
        } else if (k->key() == "y0") {
          start.y = static_cast<model::Coord>(k->value().as_int64());
        } else if (k->key() == "x1") {
          vertical = false;
          end = static_cast<model::Coord>(k->value().as_int64());
        } else if (k->key() == "y1") {
          end = static_cast<model::Coord>(k->value().as_int64());
        }
      }

      if (vertical) {
        map.AddRoad(model::Road(model::Road::VERTICAL, start, end));
      } else {
        map.AddRoad(model::Road(model::Road::HORIZONTAL, start, end));
      }
    }
  } catch (std::exception& e) {
    std::cout << "Fail to parse road" << e.what() << std::endl;
    throw;
  }
}

void parseBuilding(const boost::json::array& arr, model::Map& map) {
  if (arr.size() < 1) return;  // The array must contain at least one element

  for (auto i : arr) {
    model::Rectangle rec{{0, 0}, {0, 0}};

    auto r = i.as_object();

    for (auto k = r.begin(); k != r.end(); ++k) {
      if (k->key() == "x") {
        rec.position.x = static_cast<model::Coord>(k->value().as_int64());
      } else if (k->key() == "y") {
        rec.position.y = static_cast<model::Coord>(k->value().as_int64());
      } else if (k->key() == "w") {
        rec.size.width = static_cast<model::Coord>(k->value().as_int64());
      } else if (k->key() == "h") {
        rec.size.height = static_cast<model::Coord>(k->value().as_int64());
      }
    }

    if (rec.size.width == 0 || rec.size.height == 0) {
      throw std::logic_error("wrong building data");
    }

    map.AddBuilding(model::Building(rec));
  }
}

void parseOffice(const boost::json::array& arr, model::Map& map) {
  if (arr.size() < 1) return;  // The array must contain at least one element

  for (auto i : arr) {
    model::Point position{};
    model::Offset offset{};
    model::Office::Id id(std::string(i.as_object().at("id"sv).as_string()));

    auto r = i.as_object();
    for (auto k = r.begin(); k != r.end(); ++k) {
      if (k->key() == "x") {
        position.x = static_cast<model::Coord>(k->value().as_int64());
      } else if (k->key() == "y") {
        position.y = static_cast<model::Coord>(k->value().as_int64());
      } else if (k->key() == "offsetX") {
        offset.dx = static_cast<model::Dimension>(k->value().as_int64());
      } else if (k->key() == "offsetY") {
        offset.dy = static_cast<model::Dimension>(k->value().as_int64());
      }
    }

    map.AddOffice({id, position, offset});
  }
}

void parseMap(const boost::json::value& val, model::Map& map) {
  // get field form map
  for (auto i = val.as_object().cbegin(); i != val.as_object().cend(); i++) {
    auto p_arr = i->value().if_array();
    if (i->key() == "id"sv || i->key() == "name"sv || p_arr == nullptr) {
      continue;
    } else if (i->key() == "roads"sv) {
      parseRoads(*p_arr, map);
      std::cout << "Road\n";
    } else if (i->key() == "buildings"sv) {
      parseBuilding(*p_arr, map);
      std::cout << "buildings\n";
    } else if (i->key() == "offices"sv) {
      parseOffice(*p_arr, map);
      std::cout << "offices\n";
    } else {
      std::cout << "unknon object " << i->key() << std::endl;
      throw std::logic_error("Found unknon json object"s.append(i->key()));
    }
  }
}

}  // namespace

namespace json_loader {

model::Game LoadGame(const std::filesystem::path& json_path) {
  // Загрузить содержимое файла json_path, например, в виде строки
  // Распарсить строку как JSON, используя boost::json::parse
  // Загрузить модель игры из файла

  // open file
  std::ifstream ifs(json_path);
  if (!ifs.is_open()) {
    throw std::runtime_error(("Fail to open "s).append(json_path));
  }

  // get data from file
  const std::string data((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

  ifs.close();

  boost::json::error_code ec;

  // parse
  auto doc = boost::json::parse(data, ec);
  if (ec) {
    throw std::runtime_error("Fail to json parse "s.append(ec.message()));
  }

  model::Game game;

  auto maps = doc.as_object().at("maps"sv).as_array();  // if hasn't map thow error
  // get map from maps array
  for (auto m = maps.cbegin(); m != maps.end(); ++m) {
    model::Map map(model::Map::Id(std::string(m->as_object().at("id"sv).as_string())),
                   std::string(m->as_object().at("name"sv).as_string()));

    try {
      parseMap(m->as_object(), map);
    } catch (std::exception& e) {
      std::cout << "Fail to parse map " << e.what() << std::endl;
      continue;
    }
    game.AddMap(map);
  }

  return game;
}

}  // namespace json_loader
