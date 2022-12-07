#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#pragma once

#include <boost/asio.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/json.hpp>
#include <filesystem>
#include <string_view>

#include "response_maker.h"
#include "util.h"
#include "content_type.h"

namespace http_handler {

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using namespace std::literals;

  template <typename Body, typename Allocator, typename Send>
  void FileRequest(http::request<Body, http::basic_fields<Allocator>>& req, Send& send, const std::filesystem::path& dir) {
    auto text_response = [&req, &send](http::status status, std::string_view text) {
      send(MakeResponse(status, text, req.version(), req.keep_alive()));
    };

    auto file_response = [&req, &send](http::status status, http::file_body::value_type& file, std::string_view content_type) {
      send(MakeResponse(status, file, req.version(), req.keep_alive(), content_type));
    };

    std::string target(req.target());
    if (target == "/")
      target = "/index.html";

    std::filesystem::path pf = dir;
    pf += target;

    if (!util::IsSubPath(pf, dir)) {
      return text_response(http::status::bad_request, "Permission deny, or incorrect url request"sv);
    }

    if (!std::filesystem::exists(pf) || !std::filesystem::is_regular_file(pf)) {
      return text_response(http::status::not_found, "Not found resource, or wrong request"sv);
    }

    http::file_body::value_type file;

    if (boost::system::error_code ec; file.open(pf.c_str(), boost::beast::file_mode::read, ec), ec) {
      throw std::logic_error("Wrong open file"s.append(ec.message()));
    }

    std::string ext = pf.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

    std::string_view content_type = ContentType::DICT.count(ext) ? ContentType::DICT.at(ext) : ContentType::MEDIA_UNKNOWN;

    return file_response(http::status::ok, file, content_type);
  }



}  // namespace http_handler

#endif // __FILE_HANDLER_H__