#ifndef __STATE_SERIALIZE_H__
#define __STATE_SERIALIZE_H__

#include <boost/serialization/vector.hpp>

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