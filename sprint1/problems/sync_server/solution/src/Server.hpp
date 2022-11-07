
/**
 * @file Server.hpp
 * @author Enver Kulametov (zizu.meridian@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SERVER_H__
#define __SERVER_H__

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

class Server {
 private:
  boost::asio::ip::address m_address;
  uint16_t m_port;
  boost::system::error_code m_ec;
  net::io_context m_ioc;
  std::list<std::thread> m_threads;

  /**
   * @brief Handle incomming connection
   *
   * @param socket
   */
  void handleConnection(tcp::socket& socket);
  void jointhreadsifjoinable();

 public:
  Server();
  ~Server();

  /**
   * @brief Initial server
   *
   */
  void init();

  /**
   * @brief Run server (main loop)
   *
   */
  void run();
};

#endif  // __SERVER_H__