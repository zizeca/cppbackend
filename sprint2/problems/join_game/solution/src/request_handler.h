#pragma once

#include <boost/json.hpp>
#include <filesystem>
#include <string_view>

#include "api_responser.h"
#include "application.h"
#include "http_server.h"
#include "model.h"

namespace http_handler {
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using namespace std::literals;

class RequestHandler {
 public:
  //  explicit RequestHandler(model::Game& game, std::filesystem::path path) : game_{game}, content_path_( std::filesystem::absolute(path)) {
  explicit RequestHandler(Application& app) : app_(app) {}

  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  template <typename Body, typename Allocator, typename Send>
  void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
    ApiResponseHandler a(req, send, app_);
    a.Execute();
  }

 private:
  Application& app_;
};

}  // namespace http_handler
