#ifndef __APIREQUESTHANDLER_H__
#define __APIREQUESTHANDLER_H__

#define BOOST_BEAST_USE_STD_STRING_VIEW
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <string>
#include <string_view>

#include "content_type.hpp"
#include "model.h"

namespace http_handler {

using namespace std::string_view_literals;
using namespace std::string_literals;
namespace http = boost::beast::http;
namespace json = boost::json;
using StringResponse = http::response<http::string_body>;

namespace {

struct ErrStr {
  ErrStr() = delete;
  constexpr static std::string_view MAP_NOT_FOUND = R"({"code": "mapNotFound", "message": "Map not found"})"sv;
  constexpr static std::string_view BAD_REQ = R"({"code": "badRequest", "message": "Bad request"})"sv;
};

StringResponse MakeResponse(http::status status, std::string_view body, unsigned http_version, bool keep_alive,
                            std::string_view content_type = ContentType::TEXT_HTML) {
  StringResponse response(status, http_version);
  response.set(http::field::content_type, content_type);
  response.body() = body;
  response.payload_size();
  response.keep_alive(keep_alive);
  return response;
}
}  // namespace

template <typename Body, typename Allocator>
StringResponse ApiRequestHandler(http::request<Body, http::basic_fields<Allocator>>& req, model::Game& game) {
  const auto response = [&req](http::status status, std::string_view text) {
    return MakeResponse(status, text, req.version(), req.keep_alive(), ContentType::APP_JSON);
  };

  std::string target(req.target());

  if (!target.starts_with("/api/")) {
    throw std::logic_error("Wrong api call");
  }

  if (target.starts_with("/api/v1/maps"sv)) {
    if (target == "/api/v1/maps") {
      json::array arr;
      for (auto i : game.GetMaps()) {
        json::value v = {{"id", *i.GetId()}, {"name", i.GetName()}};
        arr.push_back(v);
      }
      json::value v = arr;
      return response(http::status::ok, std::string(json::serialize(v)));
    }

    if (target.starts_with("/api/v1/maps/")) {
      std::string s = target.substr(("/api/v1/maps/"s).size());
      auto m = game.FindMap(model::Map::Id(s));
      if (m) {
        json::value v = json::value_from(*m);
        return response(http::status::ok, json::serialize(v));
      } else {
        return response(http::status::not_found, ErrStr::MAP_NOT_FOUND);
      }
    }
  }

  return response(http::status::bad_request, ErrStr::BAD_REQ);
}
}  // namespace http_handler
#endif  // __APIREQUESTHANDLER_H__