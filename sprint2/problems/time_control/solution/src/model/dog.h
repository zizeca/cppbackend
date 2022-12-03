#ifndef __DOG_H__
#define __DOG_H__

#include "model_types.h"
#include "token_generator.h"

namespace model {

using namespace std::literals;

class Dog {
 public:
  explicit Dog(const Token& token = Token(""s));

  int GetId() const;
  Token GetToken() const;

  bool SetToken(const Token& token);

  const Vector2d& GetPosition() const;
  void SetPosition(const Vector2d& pos);

  const Vector2d& GetSpeed() const;
  void SetSpeed(const Vector2d& vspeed);
  void SetDefaultSpeed(const double& speed);

  const std::string& GetDir() const;
  void SetDir(const std::string& dir);

  void Move(const Vector2d& offset);

 private:
  Token m_token;
  int m_id;

  Vector2d m_pos;
  Vector2d m_speed;
  std::string m_dir;

  double m_default_speed;
  static int ID_COUNTER;
};

}  // namespace model

#endif  // __DOG_H__