#pragma once

#include <boost/json.hpp>
#include <string_view>

#include "ApiRequestHandler.hpp"
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
  explicit RequestHandler(model::Game& game) : game_{game} {}

  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  template <typename Body, typename Allocator, typename Send>
  void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
    // as sync_server
    const auto text_response = [&req, &send](http::status status, std::string_view text) {
      send(MakeStringResponse(status, text, req.version(), req.keep_alive(), ContentType::APP_JSON));
    };
    std::string target(req.target());

    // Обработать запрос request и отправить ответ, используя send
    if (req.method_string() != "GET" && req.method_string() != "HEAD") {
      text_response(http::status::method_not_allowed, ErrStr::BAD_REQ);
      return;
    }

    // api else file
    if (target.starts_with("/api/")) {
      send(ApiRequestHandler(req, game_));
      return;   
    } else {
      // file handler
      // check path
      // send file
      send(FileRequestHandler(req));
      return;
    }

  }

 private:
  model::Game& game_;
};

}  // namespace http_handler
