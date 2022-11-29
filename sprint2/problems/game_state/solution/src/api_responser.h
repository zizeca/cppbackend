#ifndef __APIRESPONSEHANDLER_H__
#define __APIRESPONSEHANDLER_H__

#define BOOST_BEAST_USE_STD_STRING_VIEW
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <filesystem>
#include <functional>
#include <iostream>
#include <string_view>

#include "application.h"
#include "content_type.h"
#include "model.h"
#include "response_maker.h"
#include "util.h"
namespace http_handler {

using namespace std::string_view_literals;
using namespace std::string_literals;
namespace http = boost::beast::http;
namespace json = boost::json;

using StringResponse = http::response<http::string_body>;
// using FileResponse = http::response<http::file_body>;

template <typename Body, typename Allocator, typename Send>
class ApiResponseHandler {
 public:
  ApiResponseHandler(http::request<Body, http::basic_fields<Allocator>>& req, Send& send, Application& app)
      : m_req(req), m_send(send), m_target(util::url_decode(std::string(req.target()))), m_app(app) {
    if (m_target == "/") {
      m_target += "index.html";
    }
  }

  void Execute() {
    try {
      if (m_target.starts_with("/api/v1/maps")) {
        m_send(MapRequest());
      } else if (m_target == "/api/v1/game/join") {
        m_send(PlayerJoinRequest());
      } else if (m_target == "/api/v1/game/players") {
        m_send(PlayerListRequest());
      } else if (m_target == "/api/v1/game/state") {
        // todo
        // GetState();
      } else {
        m_send(text_response(http::status::bad_request, ErrStr::BAD_REQ, ContentType::APP_JSON));
      }
    } catch (const std::exception& e) {
      m_send(text_response(http::status::internal_server_error, "server request error"sv, ContentType::TEXT_HTML));
      throw;
    }
  }

  StringResponse MapRequest() {
    assert(m_target.starts_with("/api/v1/maps"));
    if (m_req.method() != http::verb::get) {
      return text_response(http::status::method_not_allowed, ErrStr::GET_INVALID, ""sv, "GET"sv);
    }

    if (m_target == "/api/v1/maps") {
      json::array arr;
      for (auto i : m_app.GetMaps()) {
        json::value v = {{"id", *i.GetId()}, {"name", i.GetName()}};
        arr.push_back(v);
      }
      json::value v = arr;
      return text_response(http::status::ok, std::string(json::serialize(v)));
    }

    if (m_target.starts_with("/api/v1/maps/")) {
      std::string s = m_target.substr(("/api/v1/maps/"s).size());
      auto m = m_app.FindMap(model::Map::Id(s));
      if (m) {
        json::value v = json::value_from(*m);
        return text_response(http::status::ok, json::serialize(v));
      } else {
        return text_response(http::status::not_found, ErrStr::MAP_NOT_FOUND);
      }
    }
    return text_response(http::status::bad_request, ErrStr::BAD_REQ);
  }

  /**
   * @brief
   * .
   * - Request
   *  - POST /api/v1/game/join HTTP/1.1
   *  - Content-Type: application/json
   *  - body = {"userName": "Harry Potter", "mapId": "azkaban"}
   * .
   * - Response if ok
   *  - HTTP/1.1 200 OK
   *  - Content-Type: application/json
   *  - Content-Length: 61
   *  - Cache-Control: no-cache
   *  - body = {"authToken":"6516861d89ebfff147bf2eb2b5153ae1","playerId":0}
   * .
   * - Response if mapId not exist / done
   *  - HTTP/1.1 404 Not found
   *  - Content-Type: application/json
   *  - Content-Length: 51
   *  - Cache-Control: no-cache
   *  - body = {"code": "mapNotFound", "message": "Map not found"}
   * .
   * - Response if userName is empty
   *  - HTTP/1.1 400 Bad request
   *  - Content-Type: application/json
   *  - Content-Length: 54
   *  - Cache-Control: no-cache
   *  - body = {"code": "invalidArgument", "message": "Invalid name"}
   * .
   * - Response if bad JSON parsing / done
   *  - HTTP/1.1 400 Bad request
   *  - Content-Type: application/json
   *  - Content-Length: 71
   *  - Cache-Control: no-cache
   *  - body = {"code": "invalidArgument", "message": "Join game request parse error"}
   * .
   * - Response if not POST
   *  - HTTP/1.1 405 Method Not Allowed
   *  - Content-Type: application/json
   *  - Allow: POST
   *  - Content-Length: 68
   *  - Cache-Control: no-cache
   *  - {"code": "invalidMethod", "message": "Only POST method is expected"}
   */
  StringResponse PlayerJoinRequest() {
    assert(m_target == "/api/v1/game/join");

    // check method
    if (m_req.method() != http::verb::post) {
      return text_response(http::status::method_not_allowed, ErrStr::POST_INVALID, CacheControl::NO_CACHE, "POST"sv);
    }

    std::string user_name;
    std::string map_id;

    // check parsing
    try {
      boost::json::value jv = boost::json::parse(m_req.body());
      user_name = jv.as_object().at("userName").as_string();
      map_id = jv.as_object().at("mapId").as_string();
    } catch (...) {
      return text_response(http::status::bad_request, ErrStr::BAD_PARSE, CacheControl::NO_CACHE);
    }

    // check if userName is empty
    if (user_name.empty()) {
      return text_response(http::status::bad_request, ErrStr::USERNAME_EMPTY, CacheControl::NO_CACHE);
    }

    // check map id exist
    if (auto map = m_app.FindMap(model::Map::Id(map_id)); map == nullptr) {
      return text_response(http::status::not_found, ErrStr::MAP_NOT_FOUND, CacheControl::NO_CACHE);
    }

    boost::json::object object;
    try {
      const model::Player& player = m_app.JoinGame(model::Map::Id(map_id), user_name);

      object = {
          {"authToken", *player.GetToken()},
          {"playerId", player.GetId()}};

    } catch (const std::exception& e) {
      return text_response(http::status::internal_server_error, "Join Game Error :( call the fixies "s + e.what(), ContentType::TEXT_HTML);
    }
    return text_response(http::status::ok, boost::json::serialize(object), CacheControl::NO_CACHE);
  }

  /**
   * @brief
   * .
   * - Request
   *   - GET /api/v1/game/players HTTP/1.1
   *   - Authorization: Bearer 6516861d89ebfff147bf2eb2b5153ae1
   *
   * - Response if ok
   *  - HTTP/1.1 200 OK
   *  - Content-Type: application/json
   *  - Content-Length: 75
   *  - Cache-Control: no-cache
   *  - {
   *  -   "0": {"name": "Harry Potter"},
   *  -   "1": {"name": "Hermione Granger"}
   *  - }
   *
   * - Response if Authorization empty or not exist
   *  - HTTP/1.1 401 Unauthorized
   *  - Content-Type: application/json
   *  - Content-Length: 70
   *  - Cache-Control: no-cache
   *  - {"code": "invalidToken", "message": "Authorization header is missing"}
   *
   * - Response if Not GET or HEAD
   *  - HTTP/1.1 405 Method Not Allowed
   *  - Content-Type: application/json
   *  - Allow: GET, HEAD
   *  - Content-Length: 54
   *  - Cache-Control: no-cache
   *  - {"code": "invalidMethod", "message": "Invalid method"}
   *
   */
  StringResponse PlayerListRequest() {
    if (!(m_req.method() == http::verb::get || m_req.method() == http::verb::head)) {
      return text_response(http::status::method_not_allowed,
                    R"({"code": "invalidMethod", "message": "Invalid method"})"sv,
                    CacheControl::NO_CACHE,
                    "GET, HEAD"sv);
    }

    std::string auth;

    // check  has a field Authorization
    if (m_req.count(http::field::authorization)) {
      auth = m_req.at(http::field::authorization);
    } else {
      return text_response(http::status::unauthorized,
                    R"({"code": "invalidToken", "message": "Authorization header is missing"})"sv,
                    CacheControl::NO_CACHE);
    }

    // check type Authorization
    const std::string scode = "Bearer ";
    if (auth.starts_with(scode)) {
      auth = auth.substr(scode.size());
    } else {
      return text_response(http::status::unauthorized,
                    R"({"code": "invalidToken", "message": "Authorization header is missing"})"sv,
                    CacheControl::NO_CACHE);
    }

    // to lower
    std::transform(auth.begin(), auth.end(), auth.begin(), [](unsigned char c) { return std::tolower(c); });

    // check is hex simbols
    if (auth.find_first_not_of("0123456789abcdef") != std::string::npos) {
      return text_response(http::status::unauthorized,
                    R"({"code": "invalidToken", "message": "Authorization header is missing"})"sv,
                    CacheControl::NO_CACHE);
    }

    // check length token
    if (auth.size() != 32) {
      return text_response(http::status::unauthorized,
                    R"({"code": "invalidToken", "message": "Authorization header is missing"})"sv,
                    CacheControl::NO_CACHE);
    }

    // try find player
    const model::Player* p = m_app.FindPlayer(model::Token(auth));
    if (p == nullptr) {
      return text_response(http::status::unauthorized,
                    R"({"code": "unknownToken", "message": "Player token has not been found"})"sv,
                    CacheControl::NO_CACHE);
    }

    boost::json::object obj;

    for (auto it = m_app.GetPlayers().cbegin(); it != m_app.GetPlayers().cend(); ++it) {
      if (&(it->GetSession()) == &(p->GetSession()))
        obj[std::to_string(it->GetId())] = boost::json::object{{"name", it->GetName()}};
    }

    return text_response(http::status::ok,
                  boost::json::serialize(obj),
                  CacheControl::NO_CACHE);
  }



 private:
  http::request<Body, http::basic_fields<Allocator>>& m_req;
  Send& m_send;
  std::string m_target;
  Application& m_app;

  StringResponse text_response(http::status status, std::string_view body, std::string_view cache_control = std::string_view(), std::string_view allow = std::string_view()) {
    return MakeResponse(status, body, m_req.version(), m_req.keep_alive(), ContentType::APP_JSON, cache_control, allow);
  }
};

}  // namespace http_handler
#endif  // __APIRESPONSEHANDLER_H__