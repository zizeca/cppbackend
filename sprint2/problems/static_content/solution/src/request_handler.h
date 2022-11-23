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
// using namespace tag_invokers;

using StringResponse = http::response<http::string_body>;

// inner ns
namespace {

struct ErrorStr {
  ErrorStr() = delete;
  constexpr static std::string_view MAP_NOT_FOUND = R"({"code": "mapNotFound", "message": "Map not found"})"sv;
  constexpr static std::string_view BAD_REQ = R"({"code": "badRequest", "message": "Bad request"})"sv;
};

inline StringResponse MakeStringResponse(http::status status, std::string_view body, unsigned http_version, bool keep_alive,
                                         std::string_view content_type = ContentType::TEXT_HTML) {
  StringResponse response(status, http_version);
  response.set(http::field::content_type, content_type);
  response.body() = body;
  response.content_length(body.size());
  response.keep_alive(keep_alive);
  return response;
}

}  // namespace

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
      text_response(http::status::method_not_allowed, ErrorStr::BAD_REQ);
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
    }

    text_response(http::status::method_not_allowed, "Invalid method"sv);
  }

 private:
  model::Game& game_;
};

}  // namespace http_handler
