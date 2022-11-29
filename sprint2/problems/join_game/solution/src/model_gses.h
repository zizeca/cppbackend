#ifndef __MODEL_GSES_H__
#define __MODEL_GSES_H__

#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "model_map.h"
#include "tagged.h"

namespace model {

namespace detail {
struct TokenTag {};
}  // namespace detail

using Token = util::Tagged<std::string, detail::TokenTag>;
using TokenHasher = util::TaggedHasher<Token>;

using namespace std::literals;

class Dog {
 public:
  explicit Dog(Token token) : m_id(++id_counter), m_player_token(token) {
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

class GameSession;

class Player {
 public:
  Player(const Token& token, const std::string &name, GameSession& gses, Dog& dog) : m_token(token), m_name(name), m_gses(gses), m_dog(dog), m_id(id_counter++) {
    std::cout << "token \'" << *token << "\' name " << m_name << std::endl; 
  }
  Token GetToken() const { return m_token; }
  int GetId() const noexcept { return m_id; }

  const GameSession& GetSession() const noexcept { return m_gses; }
  std::string GetName() const { return std::string(m_name);}

 private:
  Token m_token;
  std::string m_name;
  GameSession& m_gses;
  Dog& m_dog;
  int m_id;
  static int id_counter;
};

class PlayerTokens {
 public:
  Token GetToken() {
    std::stringstream ss;
    ss << std::hex << m_gen1() << m_gen2();
    return Token(ss.str());
  }

  const Player* FindPlayer(const Token& token) const noexcept {
    if (auto it = m_player_id_to_index.find(token); it != m_player_id_to_index.end()) {
      return &m_players.at(it->second);
    }

    for (auto& i : m_player_id_to_index) {
      std::cout << "Token " << *i.first << " and " << *token << " = " << (*i.first == *token) << std::endl;
    }
    return nullptr;
  }

  Player& AddPlayer(Player player);

  const std::vector<Player>& GetPlayers() const noexcept { return m_players; }

 private:
  // std::unordered_map<Token, Player, util::TaggedHasher<Token>> m_player;

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

  using PlayerIdToIndex = std::unordered_map<Token, size_t, TokenHasher>;

  std::vector<Player> m_players;
  PlayerIdToIndex m_player_id_to_index;
};

class GameSession {
  GameSession(const GameSession&) = delete;
  GameSession& operator=(const GameSession&) = delete;
  // GameSession(GameSession&&) = delete;
  // GameSession& operator=(GameSession&&) = delete;

 public:
  explicit GameSession(const Map& map) : m_map(map) {}
  ~GameSession() = default;

  void AddDog(Dog dog) { m_dogs.push_back(dog); }

  const std::vector<Dog>& GetDogs() const { return m_dogs; }

 private:
  const Map& m_map;
  std::vector<Dog> m_dogs;
};

}  // namespace model

#endif  // __MODEL_GSES_H__