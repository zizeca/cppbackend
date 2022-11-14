#include "request_handler.h"

namespace http_handler {

js::value invoke(model::Map const& map) {
  js::value jv;
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
  return jv;
}

}  // namespace http_handler
