#include "player_list.h"

namespace model {

PlayerList::PlayerList() : m_record() {
}

PlayerList::~PlayerList() {
}

std::optional<std::reference_wrapper<Player>> PlayerList::FindPlayer(const Token& token) {
  if (m_players.contains(token)) {
    return m_players.at(token);
  }
  return std::nullopt;
}

Player& PlayerList::CreatePlayer(const std::string& name, size_t id) {
  Token token = GenerateToken();
  auto pr = m_players.emplace(token, Player(token, name, Player::Id(id)));
  if (pr.second) {
    return m_players.at(token);
  }
  throw std::runtime_error("Fail to add or create Player");
}

void PlayerList::AddPlayer(Player&& player) {
  const Token token = player.GetToken();
  m_players.emplace(token, player);
}

const PlayerList::Container& PlayerList::GetContainer() const {
  return m_players;
}

void model::PlayerList::Update(double delta_time) {
  for(auto it = m_players.begin(); it != m_players.end();) {
    auto dog = it->second.GetDog();
    auto sess = it->second.GetSession();
    const auto& player = it->second;

    if(dog->GetDownTime() >= sess->GetRetirementTime() && m_record) {
      // record
      m_record(player.GetToken(), player.GetName(), dog->GetPoinst(), dog->GetPlayTime());
      // delete
      it = m_players.erase(it);
    } else {
      ++it;
    }
  }
}

void model::PlayerList::SetRecorder(const model::PlayerList::Record &record) {
  m_record = record;
}

}  // namespace model