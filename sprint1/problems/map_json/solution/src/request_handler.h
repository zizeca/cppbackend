#pragma once

#define BOOST_JSON_STANDALONE
#include <boost/json.hpp>
#include <string_view>

#include "ServeHttp.hpp"
#include "model.h"

namespace http_handler {
namespace beast = boost::beast;
namespace http = beast::http;
using namespace std::literals;

using StringResponse = http::response<http::string_body>;

// inner ns
namespace {

struct ErrorStr {
  ErrorStr() = delete;
  constexpr static std::string_view MAP_NOT_FOUND = R"({
  "code": "mapNotFound",
  "message": "Map not found"
})"sv;
  constexpr static std::string_view BAD_REQ = R"({
  "code": "badRequest",
  "message": "Bad request"
})"sv;

};

struct ContentType {
  ContentType() = delete;
  constexpr static std::string_view TEXT_HTML = "text/html"sv;
  constexpr static std::string_view APP_JSON = "application/json"sv;
};

StringResponse MakeStringResponse(http::status status, std::string_view body, unsigned http_version, bool keep_alive,
                                  std::string_view content_type = ContentType::TEXT_HTML) {
  StringResponse response(status, http_version);
  response.set(http::field::content_type, content_type);
  response.body() = body;
  response.content_length(body.size());
  response.keep_alive(keep_alive);
  return response;
}

/*
StringResponse HandleRequest(StringRequest&& req) {
  const auto text_response = [&req](http::status status, std::string_view text) {
    return MakeStringResponse(status, text, req.version(), req.keep_alive());
  };

  // Здесь можно обработать запрос и сформировать ответ, но пока всегда отвечаем: Hello
  if (req.method_string() == "GET" || req.method_string() == "HEAD") {
    // return text_response(http::status::ok, std::format("Hello, ", req.target().substr(1))); // not suported
    std::string s = "Hello, ";
    // s += std::string(req.target().begin() + 1, req.target().end());
    s += std::string(req.target().substr(1));
    return text_response(http::status::ok, s);
  }

  return text_response(http::status::method_not_allowed, "Invalid method"sv);
}
 */

}  // namespace

class RequestHandler {
 public:
  explicit RequestHandler(model::Game& game) : game_{game} {}

  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  template <typename Body, typename Allocator, typename Send>
  void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
    // as sync_server
    const auto text_response = [&req,&send](http::status status, std::string_view text) {
      send(MakeStringResponse(status, text, req.version(), req.keep_alive(), ContentType::APP_JSON));
    };

    // Обработать запрос request и отправить ответ, используя send
    if (req.method_string() == "GET" || req.method_string() == "HEAD") {
      std::cout << "Target " << req.target() << std::endl;
      std::string target(req.target());
      if(!target.starts_with("/api/"sv)) {
        text_response(http::status::bad_request, ErrorStr::BAD_REQ );
        return;
      }
    } else {
      text_response(http::status::method_not_allowed, "Invalid method"sv);
    }
  }

 private:
  model::Game& game_;
};

}  // namespace http_handler
