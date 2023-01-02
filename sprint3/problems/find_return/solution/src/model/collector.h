#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include <set>
#include <list>
#include <vector>
#include <memory>
#include <variant>

#include "loot.h"
#include "dog.h"
#include "geometry.h"
#include "office.h"

namespace model {


struct DogMove{
  std::shared_ptr<Dog> dog;
  Point2d nextPos;
  void UpdateMove() {
    dog->SetPosition(nextPos);
  }
};


struct CollisionResult {
    bool IsIntersect(double collect_radius) const {
        return proj_ratio >= 0 && proj_ratio <= 1 && sq_distance <= collect_radius * collect_radius;
    }

    // квадрат расстояния до точки
    double sq_distance;

    // доля пройденного отрезка
    double proj_ratio;
};



struct CollisionEvent {
    std::shared_ptr<Dog> dog;
    std::variant<Loot, Office> obj; // maybe need replace to std::shared_ptr<GameObject>
    double sq_distance;
    double time;
};

CollisionResult TryCollectPoint(Point2d a, Point2d b, Point2d c);



class Collector {
 public:


  Collector(std::list<Loot>& loots, const std::vector<Office>& offices) : m_loots(loots), m_offices(offices) {}


  void AddDogToMoveUpdate(std::shared_ptr<Dog> dog, const Point2d& nextPos) {
    m_dogs.emplace_back(dog, nextPos);
  }

  void Update() {
    CollisionEventHandler();

    for(auto& i : m_dogs) {
      i.UpdateMove();
    }
  }


 private:

  std::list<CollisionEvent> FindEvent() const;

  void CollisionEventHandler();

  std::list<Loot>& m_loots;
  const std::vector<Office>& m_offices;
  std::vector<DogMove> m_dogs;
};

}  // namespace model

#endif  // __COLLECTOR_H__