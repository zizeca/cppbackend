#ifndef __GAME_SESSION_SER_H__
#define __GAME_SESSION_SER_H__

#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#include <boost/serialization/shared_ptr.hpp>

#include <map>
#include <string>
#include <list>

#include "../model.h"
#include "loot_ser.h"
#include "player_ser.h"

namespace model {

class GameSessionSer {
 public:
  GameSessionSer() = default;

  explicit GameSessionSer(Game::SessPtrList& sess_list, PlayerList::Container& players)
      : m_sess_list(sess_list) {
    
    for(const auto& [ _, player] : players) {
      m_players.emplace_back(player);
    }
    //
    for (const auto& player : m_players) {
      map_id.push_back(*player.GetSession()->GetMap().GetId());
    }
    
    for(const auto& sess : m_sess_list){
      m_loots.insert( std::pair<std::string, std::list<Loot>>(*sess->GetMap().GetId(), sess->GetLoots()));
    }

    if (map_id.size() != players.size()) {
      throw std::logic_error("serialization logic error");
    }
  }

  template <class Archive>
  void save(Archive& ar, const unsigned int version) const {

    ar << map_id;
    ar << m_players;
    ar << m_loots;
  }

  template <class Archive>
  void load(Archive& ar, const unsigned int version) {

    ar >> map_id;
    ar >> m_players;
    ar >> m_loots;

    if (map_id.size() == m_players.size()) {
      throw std::logic_error("serialization logic error");
    }


  }

  template <class Archive>
  void serialize(Archive& ar, const unsigned int file_version) {
    boost::serialization::split_member(ar, *this, file_version);
  }

  void UpdateGame(Game& game, PlayerList& plist) {
        // restore player list & gamesession
    assert(map_id.size() == m_players.size());

    for (size_t i = 0; i < m_players.size(); ++i) {
      auto sess = game.GetSession(Map::Id(map_id[i]));
      m_players[i].SetSession(sess);
      plist.AddPlayer(std::move(m_players[i]));
    }

    // restore uncollected loots
    for (auto [id, loot_list] : m_loots) {
      auto sess = game.GetSession(Map::Id(id));
      for (auto& loot : loot_list) {
        sess->AddLoot(std::move(loot));
      }
    }
  }

 private:
  Game::SessPtrList m_sess_list;
  std::vector<Player> m_players;
  std::vector<std::string> map_id;
  std::map<std::string, std::list<Loot>> m_loots;
};

}  // namespace model

#endif  // __GAME_SESSION_SER_H__