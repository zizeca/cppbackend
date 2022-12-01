#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <memory>

#include "dog.h"
#include "token_generator.h"

namespace model {

class GameSession;

class Player {
 public:
  explicit Player(const Token& token);

  int GetId() const noexcept;

  const std::string& GetName() const noexcept;
  void SetName(const std::string& name);

  std::shared_ptr<Dog> GetDog();
  void SetDog(std::shared_ptr<Dog> dog);

  const Token& GetToken() const;

  std::shared_ptr<GameSession> GetSession();
  const std::shared_ptr<GameSession> GetSession() const;

 private:
  int m_id;
  Token m_token;
  std::string m_name;

  std::shared_ptr<GameSession> m_gs;
  std::shared_ptr<Dog> m_dog;

  static int ID_COUNTER;
};

}  // namespace model

#endif  // __PLAYER_H__