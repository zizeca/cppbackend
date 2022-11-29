#ifndef __MODEL_DOG_H__
#define __MODEL_DOG_H__

#include <string>
#include "model_token.h"

namespace model {
using namespace std::literals;

class Dog {
 public:
  Dog(Token token = Token(""s)) : m_id(++id_counter) , m_player_token(token) {
  }

  int GetId() const noexcept { return m_id; }
  const std::string& GetName() const noexcept { return m_name; }
  void SetName(std::string name) { m_name = name; }
  Token GetToken() const { return m_player_token; }
  void SetToken(Token token) { m_player_token = token; }

 private:
  int m_id;
  std::string m_name;
  Token m_player_token;
  static int id_counter;
};



}  // namespace model

#endif  // __MODEL_DOG_H__