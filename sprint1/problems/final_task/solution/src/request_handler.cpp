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
      vobj["x0"] =  static_cast<int>(i.GetStart().x);
      vobj["y0"] = static_cast<int>(i.GetStart().y);
      if (i.IsHorizontal()) {
        vobj["x1"] = static_cast<int>(i.GetEnd().x);
      } else {
        vobj["y1"] = static_cast<int>(i.GetEnd().y);
      }
      arr.push_back(boost::json::value_from(vobj));
    }
    obj["roads"] = boost::json::value_from(arr);
  }

  if (!map.GetBuildings().empty()) {
    boost::json::array arr;
    for (auto& i : map.GetBuildings()) {
      boost::json::value v;
      v = {{"x", static_cast<int>(i.GetBounds().position.x)},
           {"y", static_cast<int>(i.GetBounds().position.y)},
           {"w", static_cast<int>(i.GetBounds().size.width)},
           {"h", static_cast<int>(i.GetBounds().size.height)}};
      arr.push_back(v);
    }
    obj["buildings"] = boost::json::value_from(arr);
  }

  if (!map.GetOffices().empty()) {
    boost::json::array arr;
    for (auto& office : map.GetOffices()) {
      boost::json::value v;
      v = {{"id", *office.GetId()},
           {"x", static_cast<int>(office.GetPosition().x)},
           {"y", static_cast<int>(office.GetPosition().y)},
           {"offsetX", static_cast<int>(office.GetOffset().dx)},
           {"offsetY", static_cast<int>(office.GetOffset().dy)}};
      arr.push_back(v);
    }
    obj["offices"] = boost::json::value_from(arr);
  }
  jv = obj;
  return jv;
}

}  // namespace http_handler
