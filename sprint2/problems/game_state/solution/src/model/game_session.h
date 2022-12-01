#ifndef __MODEL_GSES_H__
#define __MODEL_GSES_H__

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "../tagged.h"
#include "dog.h"
#include "model_map.h"

namespace model {

class GameSession {
 public:
  explicit GameSession(const Map::Id& id);
  ~GameSession();

  const Map::Id& GetMapId() const noexcept;

  std::vector<std::shared_ptr<Dog>> GetDogs();

  std::shared_ptr<Dog> GetDog(const Token& token);

  void AddDog(std::shared_ptr<Dog> dog);

 private:
  const Map::Id m_mapId;

  // owner is player
  std::vector<std::weak_ptr<Dog>> m_dogs;
};

}  // namespace model

#endif  // __MODEL_GSES_H__