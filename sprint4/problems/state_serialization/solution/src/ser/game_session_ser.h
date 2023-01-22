#ifndef __GAME_SESSION_SER_H__
#define __GAME_SESSION_SER_H__

#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "../model.h"

namespace model {

class GameSessionSer {
 public:
  explicit GameSessionSer(Game& game, PlayerList& players, size_t& max_dog_id, size_t& max_player_id)
      : m_game(game),
        m_players(players),
        m_num_sess(game.GetSessionList().size()),
        m_max_dog_id(max_dog_id),
        m_max_player_id(max_player_id) {}

  template <class Archive>
  void save(Archive& ar, const unsigned int version) const {
    // get max id
    ar << m_max_dog_id;
    ar << m_max_player_id;

    // sess serialiation
    ar << game.GetSessionList().size();
    for (auto sess : m_game.GetSessionList()) {
      ar << *sess->GetMap().GetId();
      ar << sess->GetDogs();
    }

    ar << m_players.GetContainer();
  }

  template <class Archive>
  void load(Archive& ar, const unsigned int version) {
    ar >> m_max_dog_id;
    ar >> m_max_player_id;
    
    size_t count_sess{0};

    ar >> count_sess;

    for(size_t i = 0; i < count_sess; ++i) {
      std::string map_id;
      ar >> map_id;

      auto sess = m_game.GetSession(Map::Id(map_id));
      if(sess == nullptr) {
        throw std::logic_error("can not get or create session");
      }

      GameSession::DogPtrList list;
      ar >> list;

      for(auto dog : list) {
        sess->AddDog(dog);
      }

      PlayerList::Container player_list;

      ar >> player_list;

    }

  }

  template <class Archive>
  void serialize(Archive& ar, const unsigned int file_version) {
    boost::serialization::split_member(ar, *this, file_version);
  }

 private:
  Game& m_game;
  PlayerList& m_players;

  size_t m_num_sess;
  size_t & m_max_dog_id;
  size_t & m_max_player_id;
};

}  // namespace model

#endif  // __GAME_SESSION_SER_H__