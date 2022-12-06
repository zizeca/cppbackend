#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <chrono>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

using namespace std::string_view_literals;
using namespace std::string_literals;

#include "json_loader.h"
#include "model.h"
#include "ticker.h"

class Application {
  Application() = delete;
  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(const Application&) = delete;
  Application& operator=(Application&&) = delete;

 public:
  Application(boost::asio::io_context& ioc, std::filesystem::path config, std::filesystem::path dir_to_content);
  ~Application();

  void SetManualCallOfTick(bool is_manual = true);
  bool IsManualCallOfTick() const;

  void SetRandomDogSpawn(bool is_random = true);
  bool ISRandomDogSpawn() const;

  const std::filesystem::path& GetContentDir() const noexcept;

  const model::Map* FindMap(const model::Map::Id& id) const noexcept;

  const std::vector<model::Map>& GetMaps() const noexcept;

  model::Player& JoinGame(model::Map::Id id, const std::string& user_name);

  model::Player* FindPlayer(const model::Token& t);

  const model::PlayerList::Container& GetPlayers() const noexcept;

  void Update(std::chrono::milliseconds ms);

  boost::asio::strand<boost::asio::io_context::executor_type> strand;

 private:
  boost::asio::io_context& m_ioc;
  const std::filesystem::path dir_to_content_;
  model::Game m_game;
  model::PlayerList m_player_list;
  bool m_is_manual_call_of_tick;
  bool m_is_random_dog_spawn;
};

#endif  // __APPLICATION_H__