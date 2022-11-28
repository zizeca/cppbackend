#ifndef __MODEL_PLAYER_H__
#define __MODEL_PLAYER_H__

#include "model_gses.h"

namespace model {


class Player {
 public:
  Player(GameSession& gses, Dog& dog) : m_gses(gses), m_dog(dog) {}

 private:
  GameSession& m_gses;
  Dog& m_dog;
};

}  // namespace model

#endif  // __MODEL_PLAYER_H__