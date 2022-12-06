#include "model_map.h"

#include <random>

#include "../logger.h"
namespace model {

using namespace std::literals;
using namespace gm;

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

Point2d Map::GetRandPoint() const {
  if (roads_.size() == 0) {
    throw std::logic_error("Must be one or mode roads");
  }

  std::random_device rd;
  std::mt19937 gen{rd()};
  int num_r = 0;
  if (roads_.size() > 1) {
    std::uniform_int_distribution<int> dist(0, roads_.size() - 1);
    num_r = dist(gen);
  }

  Road road = roads_.at(num_r);

  Point2i start = road.GetStart();
  Point2i end = road.GetEnd();

  Point2d ret{static_cast<double>(start.x), static_cast<double>(start.y)};

  double width;
  {
    std::uniform_real_distribution<> dist(-0.4, 0.4);
    width = dist(gen);
    width = ((int)(width * 100 + .5) / 100.0);  // round two digit
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

  ret.x = ((int)(ret.x * 100 + .5) / 100.0);  // round two digit
  ret.y = ((int)(ret.y * 100 + .5) / 100.0);  // round two digit

  // Logger::LogDebug("point ("s + std::to_string(ret.x) + ", "s + std::to_string(ret.y) + ")"s, "get random width"s);

  return ret;
}

void Map::SetDogSpeed(double speed) {
  m_dog_speed = speed;
}

double Map::GetDogSpeed() const noexcept {
  return m_dog_speed;
}

bool Road::Contains(const Point2d& point) const {
  Point2i p;

  p.x = (int)(point.x + 0.5);
  p.y = (int)(point.y + 0.5);

  if (IsHorizontal()) {
    return p.y == start_.y && p.x >= std::min(start_.x, end_.x)  && p.x <=std::max(start_.x, end_.x);
  } else {
    return p.x == start_.x && p.y >= std::min(start_.y, end_.y) && p.y <= std::max(start_.y, end_.y);
  }
}

std::optional<Road> Map::GetRoadVerByPos(const Point2d& pos) const {
  auto it = std::find_if(roads_.begin(),roads_.end(),[&pos](Road r) {
    return r.IsVertical() && r.Contains(pos);
  } );
  if(it == roads_.end()){
    return std::nullopt;
  }
  return *it;
}

std::optional<Road> Map::GetRoadHorByPos(const Point2d& pos) const {
  auto it = std::find_if(roads_.begin(),roads_.end(),[&pos](Road r) {
    return r.IsHorizontal() && r.Contains(pos);
  });

  if(it == roads_.end()){
    return std::nullopt;
  }
  return *it;
}

}  // namespace model