#pragma once

#include <boost/json.hpp>
#include <string_view>

#include "ServeHttp.hpp"
#include "model.h"
// #include "tag_invokers.h"

namespace http_handler {
namespace beast = boost::beast;
namespace http = beast::http;
namespace js = boost::json;
using namespace std::literals;
// using namespace tag_invokers;

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

}  // namespace

// костыли,
js::value invoke(model::Map const& map);

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

    // Обработать запрос request и отправить ответ, используя send
    if (req.method_string() != "GET" && req.method_string() != "HEAD") {
      text_response(http::status::method_not_allowed, ErrorStr::BAD_REQ);
      return;
    }

    std::string target(req.target());

    if (!target.starts_with("/api/v1/maps"sv)) {
        text_response(http::status::bad_request, ErrorStr::BAD_REQ);
        return;
    }
    
    if (target == "/api/v1/maps") {
        boost::json::array arr;
        for (auto i : game_.GetMaps()) {
          boost::json::value v = {{"id", *i.GetId()}, {"name", i.GetName()}};
          arr.push_back(v);
        }
        boost::json::value v = arr;
        text_response(http::status::ok, std::string(boost::json::serialize(v)));
        return;
    } 
    
    if (target.starts_with("/api/v1/maps/")) {
      std::string s = target.substr(("/api/v1/maps/"s).size());
      auto m = game_.FindMap(model::Map::Id(s));
      if (m) {
        // js::value v = js::value_from(*m);
        js::value v = invoke(*m);
        text_response(http::status::ok, js::serialize(v));
        return;
      } else {
        text_response((http::status)404, ErrorStr::MAP_NOT_FOUND);
        return;
      }
    }

    text_response(http::status::method_not_allowed, "Invalid method"sv);

  }

 private:
  model::Game& game_;
};

}  // namespace http_handler
