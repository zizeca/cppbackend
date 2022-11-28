#ifndef __MODEL_DOG_H__
#define __MODEL_DOG_H__

#include "model_token.h"

namespace model
{
  
class Dog {
 public:
  using Id = util::Tagged<std::string, Dog>;
  const Id& GetId() const noexcept { return m_id; }
  const std::string& GetName() const noexcept { return m_name; }

 private:
  Id m_id;
  std::string m_name;
  Token m_player_token;
};



} // namespace model




#endif // __MODEL_DOG_H__