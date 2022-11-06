/**
 * @file main.cpp
 * @author Enver Kulametov (zizu.meridian@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-06
 *
 * @copyright Copyright (c) 2022
 *
 */

// Boost.Beast будет использовать std::string_view вместо boost::string_view
// #define BOOST_BEAST_USE_STD_STRING_VIEW

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>

namespace net = boost::asio;
using tcp = net::ip::tcp;
using namespace std::literals;

namespace beast = boost::beast;
namespace http = beast::http;
// Запрос, тело которого представлено в виде строки
using StringRequest = http::request<http::string_body>;
// Ответ, тело которого представлено в виде строки
using StringResponse = http::response<http::string_body>;

std::optional<StringRequest> ReadRequest(tcp::socket& socket, beast::flat_buffer& buffer) {
  beast::error_code ec;
  StringRequest req;
  // Считываем из socket запрос req, используя buffer для хранения данных.
  // В ec функция запишет код ошибки.
  http::read(socket, buffer, req, ec);

  if (ec == http::error::end_of_stream) {
    return std::nullopt;
  }
  if (ec) {
    throw std::runtime_error("Failed to read request: "s.append(ec.message()));
  }
  return req;
}

void DumpRequest(const StringRequest& req) {
  std::cout << req.method_string() << ' ' << req.target() << std::endl;
  // Выводим заголовки запроса
  for (const auto& header : req) {
    std::cout << "  "sv << header.name_string() << ": "sv << header.value() << std::endl;
  }
}

void HandleConnection(tcp::socket& socket) {
  try {
    // Буфер для чтения данных в рамках текущей сессии.
    beast::flat_buffer buffer;

    while (auto request = ReadRequest(socket, buffer)) {
      DumpRequest(*request);

      // Формируем ответ со статусом 200 и версией равной версии запроса
      StringResponse response(http::status::ok, request->version());
      // Добавляем заголовок Content-Type: text/html
      response.set(http::field::content_type, "text/html"sv);
      response.body() = "<strong>Hello</strong>"sv;
      // Формируем заголовок Content-Length, сообщающий длину тела ответа
      // response.content_length(response.body().size());
      response.content_length(response.body().size() / 2);
      // Формируем заголовок Connection в зависимости от значения заголовка в запросе
      response.keep_alive(request->keep_alive());

      // Отправляем ответ сервера клиенту
      http::write(socket, response);

      // Прекращаем обработку запросов, если семантика ответа требует это
      if (response.need_eof()) {
        break;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  beast::error_code ec;
  // Запрещаем дальнейшую отправку данных через сокет
  socket.shutdown(tcp::socket::shutdown_send, ec);
}

int main() {
  net::io_context ioc;

  const auto address = net::ip::make_address("0.0.0.0");
  constexpr unsigned short port = 8080;

  tcp::acceptor acceptor(ioc, {address, port});

  while (true) {
    tcp::socket socket(ioc);
    acceptor.accept(socket);
    HandleConnection(socket);
  }

  return 0;
}