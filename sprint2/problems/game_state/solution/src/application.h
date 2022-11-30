#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <filesystem>
#include <string>
#include <string_view>
#include <optional>

using namespace std::string_view_literals;
using namespace std::string_literals;

#include "json_loader.h"
#include "model/model.h"

class Application {
  Application() = delete;
  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(const Application&) = delete;
  Application& operator=(Application&&) = delete;

 public:
  Application(boost::asio::io_context& ioc, std::filesystem::path config, std::filesystem::path dir_to_content);
  ~Application();


  const std::filesystem::path& GetContentDir() const noexcept;

  const model::Map* FindMap(const model::Map::Id& id) const noexcept;

  const std::vector<model::Map>& GetMaps() const noexcept;

  model::Player& JoinGame(model::Map::Id id, const std::string &user_name);

  model::Player* FindPlayer(const model::Token &t);

  const std::vector<model::Player>& GetPlayers() const noexcept;

  
 private:
  boost::asio::io_context& m_ioc;
  const std::filesystem::path dir_to_content_;
  model::Game m_game;
  model::PlayerTokens m_ptokens;
  
};

#endif  // __APPLICATION_H__