#include "application.h"

Application::Application(boost::asio::io_context &ioc, std::filesystem::path config, std::filesystem::path dir_to_content) : m_ioc(ioc), dir_to_content_(dir_to_content) {
  if (!(std::filesystem::exists(config) && std::filesystem::exists(dir_to_content_))) {
    throw std::logic_error("Wrong path");  //? maybe need more output information
  }
  m_game = json_loader::LoadGame(config);
}



Application::~Application() {
  // todo
  // close resource
}

const std::filesystem::path &Application::GetContentDir() const noexcept {
  return dir_to_content_;
}

const model::Map *Application::FindMap(const model::Map::Id &id) const noexcept {
  return m_game.FindMap(id);
}

const model::Game::Maps &Application::GetMaps() const noexcept {
  return m_game.GetMaps();
}

model::Player& Application::JoinGame(model::Map::Id id, const std::string &user_name){

  auto& sess = m_game.GetSession(id);


  model::Token token = m_ptokens.GetToken();

  model::Dog dog(token);
  model::Player player(token, user_name , sess, dog);
  
  return m_ptokens.AddPlayer(player);
 
}


model::Player* Application::FindPlayer(const model::Token &t) {

  return m_ptokens.FindPlayer(t);
  
}

const std::vector<model::Player>& Application::GetPlayers() const noexcept { 
  return m_ptokens.GetPlayers(); 
}
