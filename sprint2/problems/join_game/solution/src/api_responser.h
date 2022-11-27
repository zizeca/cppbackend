#ifndef __APIRESPONSEHANDLER_H__
#define __APIRESPONSEHANDLER_H__

#define BOOST_BEAST_USE_STD_STRING_VIEW
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <filesystem>
#include <functional>
#include <string_view>

#include "application.h"
#include "content_type.h"
#include "model.h"
#include "util.h"

namespace http_handler {

using namespace std::string_view_literals;
using namespace std::string_literals;
namespace http = boost::beast::http;
namespace json = boost::json;

using StringResponse = http::response<http::string_body>;
using FileResponse = http::response<http::file_body>;

struct ErrStr {
  ErrStr() = delete;
  constexpr static std::string_view MAP_NOT_FOUND = R"({"code": "mapNotFound", "message": "Map not found"})"sv;
  constexpr static std::string_view BAD_REQ = R"({"code": "badRequest", "message": "Bad request"})"sv;
};

template <typename Body, typename Allocator, typename Send>
class ApiResponseHandler {
 public:
  ApiResponseHandler(http::request<Body, http::basic_fields<Allocator>>& req, Send& send, Application& app)
      : m_req(req), m_send(send), m_target(util::url_decode(std::string(req.target()))), app_(app) {
    if (m_target == "/") {
      m_target += "index.html";
    }
  }

  void Execute() {
    try {
      if (m_req.method() == http::verb::get || m_req.method() == http::verb::head) {
        if (m_target.starts_with("/api/")) {
          ApiRequest();
        } else {
          FileRequest();
        }
      } else if (m_req.method() == http::verb::post) {
        if (m_req.count(http::field::content_type) && m_req.at(http::field::content_type) == ContentType::APP_JSON) {
          PlayerJoinRequestPost();
        } else {
          text_response(http::status::method_not_allowed, ErrStr::BAD_REQ, ContentType::APP_JSON);
        }
      } else {
        text_response(http::status::method_not_allowed, ErrStr::BAD_REQ, ContentType::APP_JSON);
      }

    } catch (const std::exception& e) {
      text_response(http::status::internal_server_error, "server request error", ContentType::TEXT_HTML);
      throw;
    }
  }

  void ApiRequest() {
    if (m_target == "/api/v1/maps") {
      json::array arr;
      for (auto i : app_.GetGame().GetMaps()) {
        json::value v = {{"id", *i.GetId()}, {"name", i.GetName()}};
        arr.push_back(v);
      }
      json::value v = arr;
      text_response(http::status::ok, std::string(json::serialize(v)), ContentType::APP_JSON);
      return;
    }

    if (m_target.starts_with("/api/v1/maps/")) {
      std::string s = m_target.substr(("/api/v1/maps/"s).size());
      auto m = app_.GetGame().FindMap(model::Map::Id(s));
      if (m) {
        json::value v = json::value_from(*m);
        text_response(http::status::ok, json::serialize(v), ContentType::APP_JSON);
        return;
      } else {
        text_response(http::status::not_found, ErrStr::MAP_NOT_FOUND, ContentType::APP_JSON);
        return;
      }
    }
    text_response(http::status::bad_request, ErrStr::BAD_REQ, ContentType::APP_JSON);
  }

  void FileRequest() {
    std::filesystem::path pf = app_.GetContentDir();
    pf += m_target;

    if (!util::IsSubPath(pf, app_.GetContentDir())) {
      return text_response(http::status::bad_request, "Permission deny, or incorrect url request", ContentType::TEXT_PLAIN);
    }

    if (!std::filesystem::exists(pf) || !std::filesystem::is_regular_file(pf)) {
      return text_response(http::status::not_found, "Not found resource, or wrong request", ContentType::TEXT_PLAIN);
    }

    boost::beast::http::file_body::value_type file;

    if (boost::system::error_code ec; file.open(pf.c_str(), boost::beast::file_mode::read, ec), ec) {
      throw std::logic_error("Wrong open file"s.append(ec.message()));
    }

    std::string ext = pf.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

    std::string_view content_type = ContentType::DICT.count(ext) ? ContentType::DICT.at(ext) : ContentType::MEDIA_UNKNOWN;

    return file_response(http::status::ok, file, content_type);
  }

  void PlayerJoinRequestPost() {
    text_response(http::status::ok, "Player name", ContentType::TEXT_HTML, "no-cache");
  }

 private:
  http::request<Body, http::basic_fields<Allocator>>& m_req;
  Send& m_send;
  std::string m_target;
  Application& app_;

  void
  text_response(http::status status, std::string_view body, std::string_view content_type, std::string_view cache_control = ""sv) {
    StringResponse response(status, m_req.version());
    response.set(http::field::content_type, content_type);
    response.body() = body;
    response.prepare_payload();
    response.keep_alive(m_req.keep_alive());
    if (!cache_control.empty()) {
      response.set(http::field::cache_control, cache_control);
    }
    m_send(response);
  }

  void file_response(http::status status, http::file_body::value_type& body, std::string_view content_type) {
    FileResponse response(status, m_req.version());
    response.set(http::field::content_type, content_type);
    response.body() = std::move(body);
    response.prepare_payload();
    response.keep_alive(m_req.keep_alive());
    m_send(response);
  }
};

}  // namespace http_handler
#endif  // __APIRESPONSEHANDLER_H__