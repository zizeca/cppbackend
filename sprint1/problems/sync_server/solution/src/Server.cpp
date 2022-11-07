
#include "Server.hpp"
#include "Client.hpp"
Server::Server() {}

Server::~Server() {
  try {
    for (auto& i : m_threads) {
      i.join();
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}

void Server::init() {
  m_address = net::ip::make_address("0.0.0.0");
  m_port = 8080;
}

void Server::run() {
  tcp::acceptor acceptor(m_ioc, tcp::endpoint(m_address, m_port));

  while (true) {
    tcp::socket socket(m_ioc);
    acceptor.accept(socket, m_ec);
    if (m_ec) {
      throw std::runtime_error(m_ec.message());
    }

    m_threads.emplace_back(&Server::handleConnection, this, std::ref(socket));

    break;
  }
}

void Server::handleConnection(tcp::socket& socket) {
  //
  std::cout << "Incomming connectiona \n";
  try {
    Client client(socket);
    client.run();
  } catch (std::exception& e) {
    std::cout << "Client error " << e.what() << std::endl;
  }
}

void Server::jointhreadsifjoinable() {
  m_threads.remove_if([](std::thread& t) {
    if (t.joinable()) {
      t.join();
      return true;
    }
    return false;
  });
}
