/**
 * @file main.cpp
 * @author Enver Kulametov (zizu.meridian@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <array>
#include <boost/asio.hpp>
#include <cstring>
#include <iostream>

#include "audio.h"

using namespace std::literals;

namespace {}

int main(int argc, char** argv) {
  using boost::asio::ip::udp;

  const size_t max_buffer_size = 65000;
  int port;
  bool is_server = false;
  boost::system::error_code ec;
  boost::asio::io_context io_context;

  if (argc == 3 && strcmp(argv[1], "server") == 0) {
    is_server = true;
    port = atoi(argv[2]);
  } else if (argc == 4 && strcmp(argv[1], "client") == 0) {
    std::cout << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << "\n";
    boost::asio::ip::address::from_string(argv[2], ec);
    if (ec) {
      std::cerr << ec.message() << std::endl;
      return 1;
    }
    port = atoi(argv[3]);
  } else {
    std::cout << "Wrong args, please write <" << argv[0] << "> [client] [ip] [port], or <" << argv[0]
              << "> [server] [port]\n";
    return 1;
  }

  if (port <= 0 || port > UINT16_MAX) {
    std::cout << "Port is wrong.\n";
    return 1;
  }
  // end check argv

  /*
  udp::socket socket(io_context);
  if (is_server) {
    socket.connect(udp::endpoint(udp::v4(), port), ec);
  } else {
    socket.open(udp::v4(), ec);
  }
  if (ec) {
    std::cout << "Socket failure \n";
    return 1;
  }
  */

  if (is_server) {
    // server part ________________________________

    udp::socket socket(io_context, udp::endpoint(udp::v4(), port));

    Player player(ma_format_u8, 1);
    while (true) {
      std::array<char, max_buffer_size> recv_buf;
      udp::endpoint remote_endpoint;
      std::cout << "Liste ... " << port << "\n";

      auto size = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
      std::cout << "Income data " << size << " bytes.\n";
      player.PlayBuffer(recv_buf.data(), size, 1.5s);
    }

  } else {
    // client part ________________________________
    udp::socket socket(io_context, udp::v4());
    auto endpoint = udp::endpoint(boost::asio::ip::address_v4::from_string(argv[2], ec), port);

    Recorder recorder(ma_format_u8, 1);
    std::string str;
    std::cout << "Press Enter to record and send message..." << std::endl;
    std::getline(std::cin, str);
    auto rec_result = recorder.Record(65000, 1.5s);
    std::cout << "Record " << rec_result.frames << " bytes.\n";
    try {
      socket.send_to(boost::asio::buffer(rec_result.data.data(), rec_result.frames), endpoint, 0, ec);
      if (ec) {
        std::cout << "Error\n";
      }

    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    } catch (...) {
      std::cout << "Sending error...\n";
    }
  }

  /* old code

      while (true) {
          std::string str;

          std::cout << "Press Enter to record message..." << std::endl;
          std::getline(std::cin, str);

          auto rec_result = recorder.Record(65000, 1.5s);
          std::cout << "Recording done" << std::endl;

          player.PlayBuffer(rec_result.data.data(), rec_result.frames, 1.5s);
          std::cout << "Playing done" << std::endl;
      }

  */
  return 0;
}
