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

model::Token Application::JoinGame(model::Map::Id id, std::string_view user_name){
  // todo  

  return m_ptoken.GetToken();
}