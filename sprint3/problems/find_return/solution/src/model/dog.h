#ifndef __DOG_H__
#define __DOG_H__

#include "geometry.h"
#include "token_generator.h"
#include "game_object.h"

namespace model {

using namespace std::literals;

class Dog : public GameObject{
 public:
  explicit Dog(const Token& token = Token(""s));

  int GetId() const;

  Token GetToken() const;

  const Point2d& GetSpeed() const noexcept;

  void Stop();

  void SetDefaultSpeed(const double& speed);

  void SetDir(const std::string& dir);
  const std::string& GetDir() const noexcept;

 private:
  Token m_token;
  int m_id;

  Point2d m_speed;
  std::string m_dir;

  double m_default_speed;
  static int ID_COUNTER;
};

}  // namespace model

#endif  // __DOG_H__