
/**
 * @file Client.hpp
 * @author Enver Kulametov (zizu.meridian@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <list>
#include <optional>
#include <thread>

namespace net = boost::asio;
using tcp = net::ip::tcp;
using namespace std::literals;
namespace beast = boost::beast;
namespace http = beast::http;

class Client {
 private:
  tcp::socket& m_socket;

 public:
  explicit Client(tcp::socket& socket);
  ~Client();
  
  void run();
};

#endif  // __CLIENT_H__