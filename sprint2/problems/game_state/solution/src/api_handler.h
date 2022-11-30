#ifndef __API_HANDLER_H__
#define __API_HANDLER_H__

#define BOOST_BEAST_USE_STD_STRING_VIEW
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <string_view>

#include "application.h"
#include "content_type.h"

namespace http_handler {
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using namespace std::literals;

using StringResponse = http::response<http::string_body>;
using StringRequest = http::request<http::string_body>;

json::value JsAnswer(std::string code, std::string message) {
  return json::object{{"code", code}, {"message", message}};
}

class ApiHandler {
 public:
  ApiHandler(Application& app, const StringRequest& req) : m_app(app), m_req(req), m_target(req.target()) {
    assert(m_target.starts_with("/api/"));
  }

  StringResponse Response() {
    try {
      if (m_target.starts_with("/api/v1/maps")) {
        return MapRequest();
      } else if (m_target == "/api/v1/game/join") {
        return PlayerJoinRequest();
      } else if (m_target == "/api/v1/game/players") {
        return PlayerListRequest();
      } else if (m_target == "/api/v1/game/state") {
        // todo
        return GetGameState();
      } else {
        return MakeJsonResponse(http::status::bad_request, {{"code", "badRequest"}, {"message", "Bad request"}});
      }
    } catch (const std::exception& e) {
      return MakeJsonResponse(http::status::internal_server_error, {{"code", "exception"}, {"message", "Has except"}});
    }

    return MakeJsonResponse(http::status::bad_request, {{"code", "badRequest"}, {"message", "Bad request"}});
  }

 private:
  Application& m_app;
  const StringRequest& m_req;
  const std::string m_target;

  StringResponse MapRequest() {
    assert(m_target.starts_with("/api/v1/maps"));
    if (m_req.method() != http::verb::get) {
      return MakeJsonResponse(http::status::method_not_allowed, JsAnswer("invalidMethod", "Only GET method is expected"), ""sv, "GET"sv);
    }

    if (m_target == "/api/v1/maps"s) {
      json::array arr;
      for (auto i : m_app.GetMaps()) {
        json::value v = {{"id", *i.GetId()}, {"name", i.GetName()}};
        arr.push_back(v);
      }
      return MakeJsonResponse(http::status::ok, arr);
    }

    if (m_target.starts_with("/api/v1/maps/")) {
      std::string s = m_target.substr(("/api/v1/maps/"s).size());
      auto m = m_app.FindMap(model::Map::Id(s));
      if (m) {
        return MakeJsonResponse(http::status::ok, json::value_from(*m));
      } else {
        return MakeJsonResponse(http::status::not_found, JsAnswer("mapNotFound", "Map not found"));
      }
    }
    return MakeJsonResponse(http::status::bad_request, JsAnswer("badRequest", "Bad request"));
  }

  StringResponse PlayerJoinRequest() {
    assert(m_target == "/api/v1/game/join");

    // check method
    if (m_req.method() != http::verb::post) {
      return MakeJsonResponse(http::status::method_not_allowed, {{"code", "invalidMethod"}, {"message", "Only POST method is expected"}}, CacheControl::NO_CACHE, "POST"sv);
    }

    std::string user_name;
    std::string map_id;

    // check parsing
    try {
      boost::json::value jv = boost::json::parse(m_req.body());
      user_name = jv.as_object().at("userName").as_string();
      map_id = jv.as_object().at("mapId").as_string();
    } catch (...) {
      return MakeJsonResponse(http::status::bad_request, {{"code", "invalidArgument"}, {"message", "Join game request parse error"}}, CacheControl::NO_CACHE);
    }

    // check if userName is empty
    if (user_name.empty()) {
      return MakeJsonResponse(http::status::bad_request, {{"code", "invalidArgument"}, {"message", "Invalid name"}}, CacheControl::NO_CACHE);
    }

    // check map id exist
    if (auto map = m_app.FindMap(model::Map::Id(map_id)); map == nullptr) {
      return MakeJsonResponse(http::status::not_found, {{"code", "mapNotFound"}, {"message", "Map not found"}}, CacheControl::NO_CACHE);
    }

    boost::json::object object;
    try {
      const model::Player& player = m_app.JoinGame(model::Map::Id(map_id), user_name);

      object = {
          {"authToken", *player.GetToken()},
          {"playerId", player.GetId()}};

    } catch (const std::exception& e) {
      return MakeJsonResponse(http::status::internal_server_error, {{"code", "exception"}, {"message", "Join Game Error :( call the fixies "s + e.what()}});
    }
    return MakeJsonResponse(http::status::ok, object, CacheControl::NO_CACHE);
  }

  StringResponse PlayerListRequest() {
    if (!(m_req.method() == http::verb::get || m_req.method() == http::verb::head)) {
      return MakeJsonResponse(http::status::method_not_allowed,
                              JsAnswer("invalidMethod", "Invalid method"),
                              CacheControl::NO_CACHE,
                              "GET, HEAD"sv);
    }

    return ExecuteAuthorized([this](model::Player& p) {
      boost::json::object obj;

      for (auto it = m_app.GetPlayers().cbegin(); it != m_app.GetPlayers().cend(); ++it) {
        if (&(it->GetSession()) == &(p.GetSession()))
          obj[std::to_string(it->GetId())] = {{"name", it->GetName()}};
      }

      return MakeJsonResponse(http::status::ok,
                              obj,
                              CacheControl::NO_CACHE);
    });
  }

  StringResponse GetGameState() {
    if (!(m_req.method() == http::verb::get || m_req.method() == http::verb::head)) {
      return MakeJsonResponse(http::status::method_not_allowed,
                              JsAnswer("invalidMethod", "Invalid method"),
                              CacheControl::NO_CACHE,
                              "GET, HEAD"sv);
    }

    return ExecuteAuthorized([this](model::Player& p) {
      boost::json::object obj;

      for (auto it = m_app.GetPlayers().cbegin(); it != m_app.GetPlayers().cend(); ++it) {
        if (&(it->GetSession()) == &(p.GetSession()))
          obj[std::to_string(it->GetId())] = {{"name", it->GetName()}};
      }

      return MakeJsonResponse(http::status::ok,
                              obj,
                              CacheControl::NO_CACHE);
    });
  }

  /**
   * @brief
   * @return Token if can extruct
   */
  std::optional<model::Token> TryExtractToken() {
    std::string auth;  // for return value

    // check field
    if (m_req.count(http::field::authorization)) {
      auth = m_req.at(http::field::authorization);
    } else {
      return std::nullopt;
    }

    // check type Authorization
    const std::string scode = "Bearer ";
    if (auth.starts_with(scode)) {
      auth = auth.substr(scode.size());
    } else {
      return std::nullopt;
    }

    // to lower
    std::transform(auth.begin(), auth.end(), auth.begin(), [](unsigned char c) { return std::tolower(c); });

    // check is hex simbols
    if (auth.find_first_not_of("0123456789abcdef") != std::string::npos) {
      return std::nullopt;
    }

    // check length token
    if (auth.size() != 32) {
      return std::nullopt;
    }
    return model::Token(auth);
  }

  template <typename Fn>
  StringResponse ExecuteAuthorized(Fn&& action) {
    if (auto token = this->TryExtractToken()) {
      model::Player* p = m_app.FindPlayer(*token);
      if (p == nullptr) {
        return MakeJsonResponse(http::status::unauthorized,
                                {{"code", "unknownToken"}, {"message", "Player token has not been found"}},
                                CacheControl::NO_CACHE);
      }

      return action(*p);
    } else {
      return MakeJsonResponse(http::status::unauthorized,
                              {{"code", "invalidToken"}, {"message", "Authorization header is missing"}},
                              CacheControl::NO_CACHE);
    }
  }

  /**
   * @brief Create Response from
   *
   * @param status netork status
   * @param val json value
   * @param cache_control
   * @param allow
   * @return StringResponse
   */
  StringResponse MakeJsonResponse(const http::status& status,
                                  const json::value& val,
                                  std::string_view cache_control = std::string_view(),
                                  std::string_view allow = std::string_view()) {
    StringResponse response(status, m_req.version());
    response.set(http::field::content_type, "application/json"sv);
    response.body() = json::serialize(val);
    response.prepare_payload();
    response.keep_alive(m_req.keep_alive());
    // if (!cache_control.empty()) {
    response.set(http::field::cache_control, cache_control);
    //}
    // if (!allow.empty()) {
    response.set(http::field::allow, allow);
    //}
    return response;
  }
};

}  // namespace http_handler

#endif  // __API_HANDLER_H__