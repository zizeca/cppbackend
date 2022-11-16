#pragma once

#include <boost/json.hpp>
#include <filesystem>
#include <string_view>

#include "model.h"
#include "tag_invokers.h"

namespace json_loader {

model::Game LoadGame(const std::filesystem::path& json_path);

}  // namespace json_loader
