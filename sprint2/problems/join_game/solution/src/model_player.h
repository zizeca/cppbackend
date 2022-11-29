#ifndef __MODEL_PLAYER_H__
#define __MODEL_PLAYER_H__

#include <string>
#include <string_view>

#include "model_token.h"
#include "model_gses.h"
#include "model_dog.h"

namespace model {


class Player {
 public:
  Player(Token token, std::string_view name, GameSession& gses, Dog& dog) : m_token(token), m_name(name), m_gses(gses), m_dog(dog), m_id(++id_counter) {}
  Token GetToken() const {return m_token;}
  int GetId() const noexcept { return m_id; }
 private:
  Token m_token;
  std::string_view m_name;
  GameSession& m_gses;
  Dog& m_dog;
  int m_id;
  static int id_counter;

};

}  // namespace model

#endif  // __MODEL_PLAYER_H__