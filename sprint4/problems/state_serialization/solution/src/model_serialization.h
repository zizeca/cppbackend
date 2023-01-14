#ifndef __STATE_SERIALIZE_H__
#define __STATE_SERIALIZE_H__

#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
// #include <boost/serialization/optional.hpp>
#include <boost/serialization/utility.hpp>

#include "optional_serialization.h"

#include "model.h"

namespace model {

/// For Point2d
template <typename Archive>
void serialize(Archive& ar, Point2d& point, [[maybe_unused]] const unsigned version) {
  ar& point.x;
  ar& point.y;
}

/// LootType
template <typename Archive>
void serialize(Archive& ar, LootType& loot_type, [[maybe_unused]] const unsigned version) {
  ar& loot_type.name;
  ar& loot_type.file;
  ar& loot_type.type;
  ar& loot_type.rotation;
  ar& loot_type.color;
  ar& loot_type.scale;
  ar& loot_type.value;

  //! may be wrong behaviour becaus type_num depend on vector index
  ar& loot_type.type_num;
}

/// Base Class GameObject
/// Splited function
template <typename Archive>
void save(Archive& ar, const GameObject& game_object, unsigned int version) {
  Point2d pos = game_object.GetPosition();
  double width = game_object.GetWidth();
  ar << pos;
  ar << width;
}

template <typename Archive>
void load(Archive& ar, GameObject& game_object, unsigned int version) {
  Point2d pos;
  double width;
  ar >> pos;
  ar >> width;
  game_object.SetPosition(pos);
  game_object.SetWidth(width);
}

template <class Archive>
void serialize(Archive& ar, GameObject& t, const unsigned int file_version) {
  boost::serialization::split_free(ar, t, file_version);
}
/// ==============

}  // namespace model

// BOOST_SERIALIZATION_SPLIT_FREE(model::GameObject);

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

#endif  // __STATE_SERIALIZE_H__