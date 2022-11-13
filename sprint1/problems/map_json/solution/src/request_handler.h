#pragma once
#include "ServeHttp.hpp"
#include "model.h"

namespace http_handler {
namespace beast = boost::beast;
namespace http = beast::http;

class RequestHandler {
public:
    explicit RequestHandler(model::Game& game)
        : game_{game} {
    }

    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
      // Обработать запрос request и отправить ответ, используя send
      if (req.method_string() == "GET" || req.method_string() == "HEAD") {
        std::cout << "Target " << req.target() << std::endl;
      }
        
    }

private:
    model::Game& game_;
};

}  // namespace http_handler
