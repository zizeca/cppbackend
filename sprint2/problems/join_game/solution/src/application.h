#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <filesystem>
#include <string>
#include <string_view>

using namespace std::string_view_literals;
using namespace std::string_literals;

#include "json_loader.h"
#include "model.h"

class Application {
  Application() = delete;
  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(const Application&) = delete;
  Application& operator=(Application&&) = delete;

 public:
  /**
   * @brief Main Application object
   * @param config path to configuration file (file with json mop or etc.)
   * @param dir_to_content path to directory with a static files
   * @exception std::logic_error if try created object with a wrong path or incorrect files
   */
  Application(std::filesystem::path config, std::filesystem::path dir_to_content);
  ~Application();

  /**
   * @brief Get the path to directory with static files
   * @return const std::filesystem::path&
   */
  const std::filesystem::path& GetContentDir() const noexcept;

  /**
   * @brief 
   * 
   * @param id map id
   * @return const model::Map* pointer on map or null
   */
  const model::Map* FindMap(const model::Map::Id& id) const noexcept;

  /**
   * @brief Get the Maps array
   * 
   * @return const std::vector<model::Map>& list of all maps parsed from json config file
   */
  const std::vector<model::Map>& GetMaps() const noexcept;

  /**
   * @brief 
   * 
   * @param map_id 
   * @param user_name 
   */
  void JoinGame(std::string_view map_id, std::string_view user_name);
  
 private:
  model::Game game_;
  const std::filesystem::path dir_to_content_;
};

#endif  // __APPLICATION_H__