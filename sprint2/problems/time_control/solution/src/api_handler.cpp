#include "api_handler.h"

#include "content_type.h"
#include "logger.h"

namespace http_handler {

json::value JsAnswer(std::string code, std::string message) {
  return json::object{{"code", code}, {"message", message}};
}

ApiHandler::ApiHandler(Application &app, const StringRequest &req) : m_app(app), m_req(req), m_target(req.target()) {
  assert(m_target.starts_with("/api/"));
}

StringResponse ApiHandler::Response() {
  try {
    if (m_target.starts_with("/api/v1/maps")) {
      return MapRequest();
    } else if (m_target == "/api/v1/game/join") {
      return PlayerJoinRequest();
    } else if (m_target == "/api/v1/game/players") {
      return PlayerListRequest();
    } else if (m_target == "/api/v1/game/state") {
      return GetGameState();
    } else if (m_target == "/api/v1/game/player/action") {
      return PostAction();
    } else if (m_target == "/api/v1/game/tick") {
      return PostTick();
    } else {
      return MakeJsonResponse(http::status::bad_request, {{"code", "badRequest"}, {"message", "Bad request"}});
    }
  } catch (const std::exception &e) {
    // return MakeJsonResponse(http::status::internal_server_error, JsAnswer("exception", "Response except = \""s + e.what() + "\""));
    Logger::LogExit(e);
    throw;
  }

  return MakeJsonResponse(http::status::bad_request, {{"code", "badRequest"}, {"message", "Bad request"}});
}

StringResponse ApiHandler::MapRequest() {
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
    return MakeJsonResponse(http::status::ok, arr, CacheControl::NO_CACHE);
  }

  if (m_target.starts_with("/api/v1/maps/")) {
    std::string s = m_target.substr(("/api/v1/maps/"s).size());
    auto m = m_app.FindMap(model::Map::Id(s));
    if (m) {
      return MakeJsonResponse(http::status::ok, json::value_from(*m), CacheControl::NO_CACHE);
    } else {
      return MakeJsonResponse(http::status::not_found, JsAnswer("mapNotFound", "Map not found"));
    }
  }
  return MakeJsonResponse(http::status::bad_request, JsAnswer("badRequest", "Bad request"));
}

StringResponse ApiHandler::PlayerJoinRequest() {
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
    return MakeJsonResponse(http::status::bad_request, JsAnswer("invalidArgument", "Join game request parse error"), CacheControl::NO_CACHE);
  }

  // check if userName is empty
  if (user_name.empty()) {
    return MakeJsonResponse(http::status::bad_request, JsAnswer("invalidArgument", "Invalid name"), CacheControl::NO_CACHE);
  }

  // check map id exist
  if (auto map = m_app.FindMap(model::Map::Id(map_id)); map == nullptr) {
    return MakeJsonResponse(http::status::not_found, JsAnswer("mapNotFound", "Map not found"), CacheControl::NO_CACHE);
  }

  boost::json::object object;
  try {
    const model::Player &player = m_app.JoinGame(model::Map::Id(map_id), user_name);

    object = {
        {"authToken", *player.GetToken()},
        {"playerId", player.GetId()}};
  } catch (const std::exception &e) {
    return MakeJsonResponse(http::status::internal_server_error, JsAnswer("exception", "Join Game Error :( call the fixies "s + e.what()));
  }
  return MakeJsonResponse(http::status::ok, object, CacheControl::NO_CACHE);
}

StringResponse ApiHandler::PlayerListRequest() {
  if (!(m_req.method() == http::verb::get || m_req.method() == http::verb::head)) {
    return MakeJsonResponse(http::status::method_not_allowed,
                            JsAnswer("invalidMethod", "Invalid method"),
                            CacheControl::NO_CACHE,
                            "GET, HEAD"sv);
  }

  return ExecuteAuthorized([this](model::Player &p) {
    boost::json::object obj;

    for (auto it = m_app.GetPlayers().cbegin(); it != m_app.GetPlayers().cend(); ++it) {
      if (it->second.GetSession() == p.GetSession())
        obj[std::to_string(it->second.GetId())] = {{"name", it->second.GetName()}};
    }

    return MakeJsonResponse(http::status::ok,
                            obj,
                            CacheControl::NO_CACHE); });
}

StringResponse ApiHandler::GetGameState() {
  if (!(m_req.method() == http::verb::get || m_req.method() == http::verb::head)) {
    return MakeJsonResponse(http::status::method_not_allowed,
                            JsAnswer("invalidMethod", "Invalid method"),
                            CacheControl::NO_CACHE,
                            "GET, HEAD"sv);
  }

  // if (!m_req.count(http::field::content_type) || m_req.at(http::field::content_type) != "application/json"sv) {
  //   return MakeJsonResponse(http::status::unauthorized,
  //                           JsAnswer("invalidArgument", "Invalid content type"),
  //                           CacheControl::NO_CACHE);
  // }

  // m_app.Update(m_app.tick);
  return ExecuteAuthorized([this](model::Player &p) {
    boost::json::object obj;

    for (auto it = m_app.GetPlayers().cbegin(); it != m_app.GetPlayers().cend(); ++it) {
      if (it->second.GetSession() == p.GetSession()) {
        obj[std::to_string(it->second.GetId())] = {
            {"pos", {it->second.GetDog()->GetPosition().x, it->second.GetDog()->GetPosition().y}},
            {"speed", {it->second.GetDog()->GetSpeed().x, it->second.GetDog()->GetSpeed().y}},
            {"dir", it->second.GetDog()->GetDir()}};
      }
    }

    return MakeJsonResponse(http::status::ok,
                            {{"players", obj}},
                            CacheControl::NO_CACHE); });
}

StringResponse ApiHandler::PostAction() {
  assert(m_target == "/api/v1/game/player/action");

  // check method
  if (m_req.method() != http::verb::post) {
    return MakeJsonResponse(http::status::method_not_allowed,
                            JsAnswer("invalidMethod", "Invalid method"),
                            CacheControl::NO_CACHE,
                            "POST"sv);
  }

  if (!m_req.count(http::field::content_type) || m_req.at(http::field::content_type) != "application/json"sv) {
    return MakeJsonResponse(http::status::unauthorized,
                            JsAnswer("invalidArgument", "Invalid content type"),
                            CacheControl::NO_CACHE);
  }

  return ExecuteAuthorized([this](model::Player &p) {
    boost::json::object obj;
    // todo
    
    try{
      boost::json::value jv = boost::json::parse(m_req.body());
      
      p.GetDog()->SetDir( static_cast<std::string>(jv.as_object().at("move").as_string()));
      // std::cout << "dog dir " << p.GetDog()->GetDir() << " id" << p.GetDog()->GetId() << std::endl;

    } catch (...) {
      return MakeJsonResponse(http::status::bad_request, JsAnswer("invalidArgument","Failed to parse action"),CacheControl::NO_CACHE );
    }

    return MakeJsonResponse(http::status::ok,
                            json::object(),
                            CacheControl::NO_CACHE); });
}

StringResponse ApiHandler::PostTick() {
  assert(m_target == "/api/v1/game/tick");
  // check method
  if (m_req.method() != http::verb::post) {
    return MakeJsonResponse(http::status::method_not_allowed, {{"code", "invalidMethod"}, {"message", "Only POST method is expected"}}, CacheControl::NO_CACHE, "POST"sv);
  }

  boost::json::object obj;
  double sec;
  try {
    boost::json::value jv = boost::json::parse(m_req.body());
    sec = jv.as_object().at("timeDelta").as_int64() / 1000.0;
  } catch (const std::exception &e) {
    return MakeJsonResponse(http::status::bad_request, JsAnswer("invalidArgument", "Failed to parse tick request JSON "s + e.what()), CacheControl::NO_CACHE);
  }

  m_app.tick = sec;
  // std::abort();
  try {
    m_app.Update(sec);
  } catch (const std::exception &e) {
    Logger::LogExit(e);
    throw;
  }

  return MakeJsonResponse(http::status::ok,
                          json::object(),
                          CacheControl::NO_CACHE);
}

std::optional<model::Token> ApiHandler::TryExtractToken() {
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

StringResponse ApiHandler::MakeJsonResponse(const http::status &status,
                                            const json::value &val,
                                            std::string_view cache_control,
                                            std::string_view allow) {
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

StringResponse ApiHandler::ExecuteAuthorized(std::function<StringResponse(model::Player &player)> action) {
  if (auto token = this->TryExtractToken()) {
    model::Player *p = m_app.FindPlayer(*token);
    if (p == nullptr) {
      return MakeJsonResponse(http::status::unauthorized,
                              JsAnswer("unknownToken", "Player token has not been found"),
                              CacheControl::NO_CACHE);
    }

    return action(*p);
  } else {
    return MakeJsonResponse(http::status::unauthorized,
                            JsAnswer("invalidToken", "Authorization header is missing"),
                            CacheControl::NO_CACHE);
  }
}

}  // namespace http_handler