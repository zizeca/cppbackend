#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include <set>
#include <list>
#include <vector>
#include <memory>

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



class Collector {
 public:


  Collector(std::list<Loot>& loots, const std::vector<Office>& offices) : m_loots(loots), m_offices(offices) {}


  void AddDogToMoveUpdate(std::shared_ptr<Dog> dog, const Point2d& nextPos) {
    m_dogs.emplace_back(dog, nextPos);
  }

  void Update() {


    for(auto& i : m_dogs) {
      i.UpdateMove();
    }
  }

 private:
  std::list<Loot>& m_loots;
  const std::vector<Office>& m_offices;
  std::vector<DogMove> m_dogs;
};

}  // namespace model

#endif  // __COLLECTOR_H__