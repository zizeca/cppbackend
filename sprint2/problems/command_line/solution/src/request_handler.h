#pragma once

#include <boost/asio.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/json.hpp>
#include <filesystem>
#include <string_view>

#include "api_handler.h"
#include "application.h"
#include "http_server.h"
#include "model.h"
#include "response_maker.h"
#include "util.h"
#include "content_type.h"
#include "file_handler.h"

namespace http_handler {
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using namespace std::literals;

class RequestHandler : public std::enable_shared_from_this<RequestHandler> {
 public:
  //  explicit RequestHandler(model::Game& game, std::filesystem::path path) : game_{game}, content_path_( std::filesystem::absolute(path)) {
  explicit RequestHandler(boost::asio::io_context& ioc, Application& app) : m_ioc(ioc), m_app(app), m_strand(boost::asio::make_strand(ioc)) {}

  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  template <typename Body, typename Allocator, typename Send>
  void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
    std::string target(req.target());
    if (target.starts_with("/api/")) {

      return boost::asio::dispatch(m_app.strand, [self = shared_from_this(), req = std::move(req), send = std::move(send)] {
        // check execute inside strand
        assert(self->m_app.strand.running_in_this_thread());
        ApiHandler a(self->m_app, req);
        send(a.Response());
      });
    } else {
      if (req.method() == http::verb::get || req.method() == http::verb::head) {
        FileRequest(req, send, m_app.GetContentDir());
        return;
      }
    }

    send(MakeResponse(http::status::bad_request, ErrStr::BAD_REQ, req.version(), req.keep_alive()));
  }

 private:
  boost::asio::io_context& m_ioc;
  Application& m_app;
  boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
};

}  // namespace http_handler
