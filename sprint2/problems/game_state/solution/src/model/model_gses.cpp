#include "model_gses.h"


namespace model
{
  



int Player::id_counter = 0;
int Dog::id_counter = 0;


Player& PlayerTokens::AddPlayer(Player player) {
  const size_t index = m_players.size();
  if (auto [it, inserted] = m_player_id_to_index.emplace(player.GetToken(), index); !inserted) {
    throw std::invalid_argument("Player with token "s + *player.GetToken() + " already exists"s);
  } else {
    try {
      return m_players.emplace_back(std::move(player));
    } catch (...) {
      m_player_id_to_index.erase(it);
      throw;
    }
  }
}





} // namespace model
