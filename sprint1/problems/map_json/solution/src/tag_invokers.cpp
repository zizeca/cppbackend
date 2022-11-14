#include "tag_invokers.h"

namespace tag_invokers {
/**/
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
  if(road.IsHorizontal()) {
    obj["x1"] = std::to_string(road.GetEnd().x);
  } else {
    obj["y1"] = std::to_string(road.GetEnd().y);
  }
  jv = obj;
}
/**/
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, model::Map const& map) {
  boost::json::object obj;
  obj["id"] = *map.GetId();
  obj["name"] = map.GetName();

  if(!map.GetRoads().empty()){
    obj["roads"] = boost::json::value_from(map.GetRoads());
  }
  if (!map.GetBuildings().empty()) {
    obj["buildings"] = boost::json::value_from(map.GetBuildings());
  }
  if (!map.GetOffices().empty()) {
    obj["offices"] = boost::json::value_from(map.GetOffices());
  }
  jv = obj;
}
/**/
}  // namespace tag_invokes