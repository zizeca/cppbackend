#include "application.h"

Application::Application(std::filesystem::path config, std::filesystem::path dir_to_content) : dir_to_content_(dir_to_content) {
  if (!(std::filesystem::exists(config) && std::filesystem::exists(dir_to_content_))) {
    throw std::logic_error("Wrong path");  //? maybe need more output information
  }
  game_ = json_loader::LoadGame(config);
}

Application::~Application(){
  // todo
  // close resource
}

const std::filesystem::path& Application::GetContentDir() const noexcept {
  return dir_to_content_;
}

model::Game& Application::GetGame() noexcept {
  return game_;
}
