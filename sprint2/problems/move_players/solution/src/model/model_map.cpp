#include "model_map.h"

#include <random>
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

Vector2d Map::GetRandPoint() {
  if (roads_.size() == 0) {
    throw std::logic_error("Must be one or mode roads");
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  Vector2d ret{0.f, 0.f};
  int num_r = 0;
  {
    std::uniform_int_distribution<int> dist(0, roads_.size());
    num_r = dist(gen);
  }

  Road road = roads_.at(num_r);

  Point start = road.GetStart();
  Point end = road.GetEnd();

  double width;
  {
    std::uniform_real_distribution<> dist(-0.4, 0.4);
    width = dist(gen);
  }

  if (road.IsHorizontal()) {
    std::uniform_real_distribution<> dist(start.x, end.x);
    ret.x = dist(gen);
    ret.y += width;
  } else {
    std::uniform_real_distribution<> dist(start.y, end.y);
    ret.y = dist(gen);
    ret.x += width;
  }
  return ret;
}

}  // namespace model