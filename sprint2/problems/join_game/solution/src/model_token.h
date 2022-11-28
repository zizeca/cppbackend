#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>
#include <sstream>
#include <random>

#include "tagged.h"

namespace model {

namespace detail {
struct TokenTag {};
}  // namespace detail

using Token = util::Tagged<std::string, detail::TokenTag>;
using TokenHasher = util::TaggedHasher<Token>;


class PlayerTokens {
 public:

  Token GetToken(){
    std::stringstream ss;
    ss << std::hex <<  m_gen1() << m_gen2();
    return Token(ss.str());
  }
 private:

  //std::unordered_map<Token, Player, util::TaggedHasher<Token>> m_player;

  std::random_device m_random_device;
  std::mt19937_64 m_gen1{[this] {
    std::uniform_int_distribution<std::mt19937_64::result_type> dist;
    return dist(m_random_device);
  }()};
  std::mt19937_64 m_gen2{[this] {
    std::uniform_int_distribution<std::mt19937_64::result_type> dist;
    return dist(m_random_device);
  }()};
  // Чтобы сгенерировать токен, получите из generator1_ и generator2_
  // два 64-разрядных числа и, переведя их в hex-строки, склейте в одну.
  // Вы можете поэкспериментировать с алгоритмом генерирования токенов,
  // чтобы сделать их подбор ещё более затруднительным
};




}  // namespace model

#endif  // __TOKEN_H__