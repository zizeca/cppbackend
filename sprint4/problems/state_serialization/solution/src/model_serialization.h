#ifndef __STATE_SERIALIZE_H__
#define __STATE_SERIALIZE_H__

#include <boost/serialization/vector.hpp>
// #include <boost/serialization/optional.hpp>
#include <boost/serialization/utility.hpp>

#include "optional_serialization.h"

#include "model.h"

namespace model
{

template <typename Archive>
void serialize(Archive& ar, Point2d& point, [[maybe_unused]] const unsigned version) {
    ar& point.x;
    ar& point.y;
}


template <typename Archive>
void serialize(Archive& ar, GameObject& game_object, [[maybe_unused]] const unsigned version) {
  ar & game_object.GetPosition();
  ar & game_object.GetWidth();
}

template <typename Archive>

void serialize(Archive& ar, LootType& loot_type, [[maybe_unused]] const unsigned version) {

  ar & loot_type.name;
  ar & loot_type.file;
  ar & loot_type.type;
  ar & loot_type.rotation;
  ar & loot_type.color;
  ar & loot_type.scale;
  ar & loot_type.value;

  //! may be wrong behaviour
  ar & loot_type.type_num; 
}

} // namespace model




// class SessSer {
//   SessSer(model::Game::SessPtrList& sess) : m_sess(sess) {}
//  public: 
//   template<class Archive>
//   void serialize(Archive &ar, const unsigned int version) {
//   }

//   model::Game::SessPtrList& m_sess;
// };


// class GameSer {
//  public:
//   explicit GameSer(model::Game &game, model::PlayerList& plist) : m_game(game) , m_plist(plist) {
//   }

//   template<class Archive>
//   void serialize(Archive &ar, const unsigned int version) {
//      ar & SessSer(const_cast<model::Game::SessPtrList&>(m_game.GetSessionList()));
//      ar & m_plist;
//   }

//  private:

//  model::Game &m_game;
//  model::PlayerList m_plist;

// };



#endif // __STATE_SERIALIZE_H__