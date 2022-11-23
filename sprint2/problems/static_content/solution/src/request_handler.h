#pragma once

#include <boost/json.hpp>
#include <filesystem>
#include <string_view>

#include "ApiResponseHandler.hpp"
#include "content_type.hpp"
#include "http_server.h"
#include "model.h"
#include "tag_invokers.h"

namespace http_handler {
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using namespace std::literals;

class RequestHandler {
 public:
  explicit RequestHandler(model::Game& game, std::filesystem::path path) : game_{game}, content_path_( std::filesystem::absolute(path)) {
    if (!std::filesystem::exists(content_path_)) {
      throw std::logic_error("path to static files not exist");
    }
  }

  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  template <typename Body, typename Allocator, typename Send>
  void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
    ApiResponseHandler a(req, send, content_path_, game_);
    a.Execute();
  }

 private:
  model::Game& game_;
  std::filesystem::path content_path_;
};

}  // namespace http_handler
