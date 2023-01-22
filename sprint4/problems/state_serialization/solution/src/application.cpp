#include "application.h"

#include <cassert>
#include <chrono>

#include "logger.h"
#include "ticker.h"
#include "model_serialization.h"

Application::Application(boost::asio::io_context &ioc, const c_parse::Args &args)
    : m_ioc(ioc),
      strand(boost::asio::make_strand(ioc)),
      m_max_player_id{0},
      m_max_dog_id{0} {
  // required pathes
  if (!(std::filesystem::exists(args.config_file) && std::filesystem::exists(args.www_root))) {
    throw std::logic_error("Wrong path, config="s + args.config_file + ", content="s + args.www_root);  //? maybe need more output information
  }

  // path to static files
  m_dir_to_content = args.www_root;

  // create game form json config
  m_game = json_loader::LoadGame(args.config_file);

  // random spawn
  m_game.SetRandomSpawn(args.random_spawn);

  // auto update
  if (args.tick_period) {
    std::make_shared<util::Ticker>(this->strand, std::chrono::milliseconds(args.tick_period), std::bind(&Application::Update, this, std::placeholders::_1))->Start();
    m_manual_ticker = false;
  } else {
    m_manual_ticker = true;
  }

  // load state
  if (std::filesystem::exists(args.state_file)) {
    LoadState(args.state_file);
  }

  // auto save
  if (!args.state_file.empty() && args.save_state_period) {
    // todo
    std::make_shared<util::Ticker>(this->strand, std::chrono::milliseconds(args.save_state_period), std::bind(&Application::SaveState, this))->Start();
  }
}

Application::~Application() {
  // todo
  // close resource
}

void Application::SetManualTicker(bool enable) {
  m_manual_ticker = enable;
}

bool Application::IsManualTicker() const {
  return m_manual_ticker;
}

void Application::SetRandomSpawn(bool enable) {
  m_game.SetRandomSpawn(enable);
}

const std::filesystem::path &Application::GetContentDir() const noexcept {
  return m_dir_to_content;
}

std::optional<std::reference_wrapper<const model::Map>> Application::FindMap(const model::Map::Id &id) const noexcept {
  return m_game.FindMap(id);
}

const model::Game::Maps &Application::GetMaps() const noexcept {
  return m_game.GetMaps();
}

model::Player &Application::JoinGame(const model::Map::Id &id, const std::string &user_name) {
  //  get if exist or get created session
  auto sess = m_game.GetSession(id);
  if (!sess) {
    throw std::runtime_error("Fail to open or create ssesion");
  }

  auto &player = m_player_list.CreatePlayer(user_name, m_max_player_id++);

  auto dog = std::make_shared<model::Dog>(model::Dog::Id(m_max_dog_id++));

  player.SetDog(dog);
  player.SetSession(sess);

  return player;
}

std::optional<std::reference_wrapper<model::Player>> Application::FindPlayer(const model::Token &t) {
  return m_player_list.FindPlayer(t);
}

const model::PlayerList::Container &Application::GetPlayers() const noexcept {
  return m_player_list.GetContainer();
}

void Application::Update(std::chrono::milliseconds ms) {
  if (!m_manual_ticker && (ms.count() > timeout)) {  // fail if timeout
    throw std::runtime_error("Time for aplication update state is very long");
  }

  const double delta_time = std::chrono::duration<double>(ms).count();
  m_game.Update(delta_time);
}

void Application::SaveState() {
  std::cout << "Noimplement Save\n";
  // boost::archive::text_oarchive oa{m_ss};

  // GameSer gs(m_game);

  // oa << gs;
  // oa << m_player_list;
}

void Application::LoadState(const std::filesystem::path &path) {
  std::ifstream file(path);
  if(!file.is_open()) {
    throw std::logic_error("Fail to open file - "s + path.string());
  }

  InputArchive input_archive{file};

  model::GameSessionSer ser;
  input_archive >> ser;

  ser.UpdateGame(m_game, m_player_list);

}
