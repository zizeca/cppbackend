#ifndef __MODEL_GSES_H__
#define __MODEL_GSES_H__

#include "model_dog.h"
#include "model_map.h"

namespace model {

class GameSession {
  GameSession(const GameSession&) = delete;
  GameSession& operator=(const GameSession&) = delete;
  GameSession(GameSession&&) = delete;
  GameSession& operator=(GameSession&&) = delete;

 public:
  explicit GameSession(Map& map) : m_map(map) {}
  ~GameSession() = default;

  void AddDog(Dog dog) { m_dogs.push_back(dog); }

 private:
  Map& m_map;
  std::vector<Dog> m_dogs;
};

}  // namespace model

#endif  // __MODEL_GSES_H__