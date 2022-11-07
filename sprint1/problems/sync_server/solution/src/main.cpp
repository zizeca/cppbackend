/**
 * @file main.cpp
 * @author Enver Kulametov (zizu.meridian@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifdef WIN32
#include <sdkddkver.h>
#endif
// boost.beast будет использовать std::string_view вместо boost::string_view
#define BOOST_BEAST_USE_STD_STRING_VIEW

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <optional>
#include <thread>

#include "Server.hpp"

namespace net = boost::asio;
using tcp = net::ip::tcp;
using namespace std::literals;
namespace beast = boost::beast;
namespace http = beast::http;

int main() {
  // Выведите строчку "Server has started...", когда сервер будет готов принимать подключения

  Server s;
  try {
    s.init();
    s.run();
  } catch (std::exception& e) {
    std::cout << "Server exception " << e.what() << std::endl;
  } catch (...) {
    std::cout << "Catch unknown error\n";
  }
}
