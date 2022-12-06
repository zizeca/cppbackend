#include "application.h"

#include <cassert>
#include <chrono>
#include "logger.h"

Application::Application(boost::asio::io_context &ioc, std::filesystem::path config, std::filesystem::path dir_to_content)
    : m_ioc(ioc), dir_to_content_(dir_to_content), strand(boost::asio::make_strand(ioc)), m_is_manual_call_of_tick(true), m_is_random_dog_spawn(false) {
  if (!(std::filesystem::exists(config) && std::filesystem::exists(dir_to_content_))) {
    throw std::logic_error("Wrong path");  //? maybe need more output information
  }
  m_game = json_loader::LoadGame(config);
}

Application::~Application() {
  // todo
  // close resource
}

void Application::SetManualCallOfTick(bool is_manual){
  m_is_manual_call_of_tick = is_manual;
}

bool Application::IsManualCallOfTick() const {
  return m_is_manual_call_of_tick;
}

void Application::SetRandomDogSpawn(bool is_random) {
  m_is_random_dog_spawn = is_random;
}
bool Application::ISRandomDogSpawn() const {
  return m_is_random_dog_spawn;
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

model::Player &Application::JoinGame(model::Map::Id id, const std::string &user_name) {
  //  get if exist or get created session
  auto sess = m_game.GetSession(id);
  if (!sess) {
    throw std::runtime_error("Fail to open or create ssesion");
  }

  auto &player = m_player_list.CreatePlayer(user_name);
  player.SetSession(sess);
  
  if(m_is_random_dog_spawn){
    player.GetDog()->SetPosition(sess->GetMap().GetRandPoint());
  }

  sess->AddDog(player.GetDog());

  return player;
}

model::Player *Application::FindPlayer(const model::Token &t) {
  return m_player_list.FindPlayer(t);
}

const model::PlayerList::Container &Application::GetPlayers() const noexcept {
  return m_player_list.GetContainer();
}

void Application::Update(std::chrono::milliseconds ms) {
  assert(ms.count() < 30000ul); // fail if more than 30s
  
  double delta = std::chrono::duration<double>(ms).count();
  // std::cout << "Call update with delta " << ms << "ms in " << delta << std::endl;
  m_game.Update(delta);
  // Logger::LogDebug("update", "__App update__ "s + std::to_string(delta));
}
