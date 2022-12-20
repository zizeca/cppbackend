#ifndef __MODEL_GSES_H__
#define __MODEL_GSES_H__

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "../tagged.h"
#include "dog.h"
#include "model_map.h"
#include "loot_generator.h"

namespace model {

class GameSession {
 public:
  explicit GameSession(const Map& map, loot_gen::LootGenerator gen);
  ~GameSession();

  const Map& GetMap() const noexcept;

  std::shared_ptr<Dog> GetDog(const Token& token);

  void AddDog(std::shared_ptr<Dog> dog);

  void Update(const double& delta);

  void SetDogRandomSpawn(bool enable = true);

 private:
  const Map& m_map;
  std::vector<std::shared_ptr<Dog>> m_dogs;
  bool m_random_spawn;

  loot_gen::LootGenerator m_loot_gen;
};

}  // namespace model

#endif  // __MODEL_GSES_H__