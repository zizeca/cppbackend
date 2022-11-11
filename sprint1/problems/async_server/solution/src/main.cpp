/**
 * @file main.cpp
 * @author Enver Kulametov (zizu.meridian@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifdef WIN32
#include <sdkddkver.h>
#endif

#define BOOST_BEAST_USE_STD_STRING_VIEW
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
// #include <format> not suported g++-12
#include <iostream>
#include <optional>
#include <thread>

#include "ServeHttp.hpp"

namespace net = boost::asio;
namespace sys = boost::system;
using namespace std::string_view_literals;
namespace http = boost::beast::http;

// Запрос, тело которого представлено в виде строки
using StringRequest = http::request<http::string_body>;
// Ответ, тело которого представлено в виде строки
using StringResponse = http::response<http::string_body>;


// Структура ContentType задаёт область видимости для констант,
// задающий значения HTTP-заголовка Content-Type
struct ContentType {
  ContentType() = delete;
  constexpr static std::string_view TEXT_HTML = "text/html"sv;
  // При необходимости внутрь ContentType можно добавить и другие типы контента
};

// Запускает функцию fn на n потоках, включая текущий
template <typename Fn>
void RunWorkers(unsigned n, const Fn& fn) {
  n = std::max(1u, n);
  std::vector<std::jthread> workers;
  workers.reserve(n - 1);
  // Запускаем n-1 рабочих потоков, выполняющих функцию fn
  while (--n) {
    workers.emplace_back(fn);
  }
  fn();
}

StringResponse MakeStringResponse(http::status status, std::string_view body, unsigned http_version, bool keep_alive,
                                  std::string_view content_type = ContentType::TEXT_HTML) {
  StringResponse response(status, http_version);
  response.set(http::field::content_type, content_type);
  response.body() = body;
  response.content_length(body.size());
  response.keep_alive(keep_alive);
  return response;
}

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


int main() {
  const unsigned num_threads = std::thread::hardware_concurrency();
  net::io_context ioc(num_threads);


  // Подписываемся на сигналы и при их получении завершаем работу сервера
  net::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait([&ioc](const sys::error_code& ec, [[maybe_unused]] int signal_number) {
    if (!ec) {
      // std::cout << "Signal "sv << signal_number << " received"sv << std::endl;
      ioc.stop();
    }
  });
  // net::steady_timer t{ioc, 30s};
  // t.async_wait([](sys::error_code ec) { std::cout << "Timer expired"s << std::endl; });


  const auto address = net::ip::make_address("0.0.0.0");
  constexpr net::ip::port_type port = 8080;

  http_server::ServeHttp(ioc, {address, port},
                         [](auto&& req, auto&& sender) { sender(HandleRequest(std::forward<decltype(req)>(req))); });

  std::cout << "Server has started..."sv << std::endl;
  RunWorkers(num_threads, [&ioc] { ioc.run(); });

  // std::cout << "Shutting down"sv << std::endl;
  return 0;
}