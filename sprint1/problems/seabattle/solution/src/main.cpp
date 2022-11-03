#ifdef WIN32
#include <sdkddkver.h>
#endif

#include <atomic>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <thread>

#include "seabattle.h"

namespace net = boost::asio;
using net::ip::tcp;
using namespace std::literals;

void PrintFieldPair(const SeabattleField& left, const SeabattleField& right) {
  auto left_pad = "  "s;
  auto delimeter = "    "s;
  std::cout << left_pad;
  SeabattleField::PrintDigitLine(std::cout);
  std::cout << delimeter;
  SeabattleField::PrintDigitLine(std::cout);
  std::cout << std::endl;
  for (size_t i = 0; i < SeabattleField::field_size; ++i) {
    std::cout << left_pad;
    left.PrintLine(std::cout, i);
    std::cout << delimeter;
    right.PrintLine(std::cout, i);
    std::cout << std::endl;
  }
  std::cout << left_pad;
  SeabattleField::PrintDigitLine(std::cout);
  std::cout << delimeter;
  SeabattleField::PrintDigitLine(std::cout);
  std::cout << std::endl;
}

template <size_t sz>
static std::optional<std::string> ReadExact(tcp::socket& socket) {
  boost::array<char, sz> buf;
  boost::system::error_code ec;

  net::read(socket, net::buffer(buf), net::transfer_exactly(sz), ec);

  if (ec) {
    return std::nullopt;
  }

  return {{buf.data(), sz}};
}

static bool WriteExact(tcp::socket& socket, std::string_view data) {
  boost::system::error_code ec;

  net::write(socket, net::buffer(data), net::transfer_exactly(data.size()), ec);

  return !ec;
}

class SeabattleAgent {
 public:
  SeabattleAgent(const SeabattleField& field) : my_field_(field) {}

  void StartGame(tcp::socket& socket, bool my_initiative) {
    while (!IsGameEnded()) {
      PrintFields();
      std::optional<std::pair<int, int>> popt;

      if (my_initiative) {
        std::cout << "You turn:";
        std::string input;

        // loop for rong input
        while (true) {
          std::cin >> input;
          popt = ParseMove(input);
          if (!popt) {
            std::cout << "Wrong turn...\n";
          } else {
            SeabattleField::State s = other_field_(popt->second, popt->first);
            if (s != SeabattleField::State::UNKNOWN) {
              std::cout << "Cell is known...\n";
            } else {
              break;
            }
          }
        }

        if (!WriteExact(socket, input)) {
          throw std::runtime_error("Fail to WriteExact");
        }

        auto retval = ReadExact<1>(socket);

        switch (static_cast<SeabattleField::ShotResult>(*retval->c_str() - '0')) {
          case SeabattleField::ShotResult::MISS:
            std::cout << "Miss!\n";
            my_initiative = !my_initiative;
            other_field_.MarkMiss(popt->second, popt->first);
            break;
          case SeabattleField::ShotResult::HIT:
            std::cout << "Hit!\n";
            other_field_.MarkHit(popt->second, popt->first);
            break;
          case SeabattleField::ShotResult::KILL:
            std::cout << "Kill!\n";
            other_field_.MarkKill(popt->second, popt->first);
            break;

          default:
            break;
        }

      } else {
        std::cout << "Wait turn...\n";
        auto retval = ReadExact<2>(socket);
        std::cout << retval.value() << std::endl;
        popt = ParseMove(retval.value());
        // std::cout << "opt " << popt->first << " " << popt->second << "\n";
        auto shootres = my_field_.Shoot(popt->second, popt->first);  // mixed :( first and second
        // std::cout << "Shoot res = " << (int)shootres << std::endl;
        if (!WriteExact(socket, std::to_string(static_cast<int>(shootres)))) {
          throw std::runtime_error("Fail to write");
        }

        switch (shootres) {
          case SeabattleField::ShotResult::MISS:
            std::cout << "Miss.\n";
            my_initiative = !my_initiative;
            break;
          case SeabattleField::ShotResult::HIT:
            std::cout << "Hit!\n";
            break;
          case SeabattleField::ShotResult::KILL:
            std::cout << "Kill!\n";
            break;

          default:
            break;
        }
      }
    }

    if (my_field_.IsLoser()) {
      std::cout << "Your Lose.\n";
    } else {
      std::cout << "Your Win.\n";
    }
  }

 private:
  static std::optional<std::pair<int, int>> ParseMove(const std::string_view& sv) {
    if (sv.size() != 2) return std::nullopt;

    int p1 = sv[0] - 'A', p2 = sv[1] - '1';

    if (p1 < 0 || p1 > 8) return std::nullopt;
    if (p2 < 0 || p2 > 8) return std::nullopt;

    return {{p1, p2}};
  }

  static std::string MoveToString(std::pair<int, int> move) {
    char buff[] = {static_cast<char>(move.first) + 'A', static_cast<char>(move.second) + '1'};
    return {buff, 2};
  }

  void PrintFields() const { PrintFieldPair(my_field_, other_field_); }

  bool IsGameEnded() const { return my_field_.IsLoser() || other_field_.IsLoser(); }

  // TODO: добавьте методы по вашему желанию

 private:
  SeabattleField my_field_;
  SeabattleField other_field_;
};

void StartServer(const SeabattleField& field, unsigned short port) {
  SeabattleAgent agent(field);

  net::io_context io_context;

  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
  std::cout << "Waiting for connection..."sv << std::endl;

  boost::system::error_code ec;
  tcp::socket socket{io_context};
  acceptor.accept(socket, ec);

  if (ec) {
    std::cout << "Can't accept connection"sv << std::endl;
    throw std::runtime_error(ec.message());
  }
  agent.StartGame(socket, false);
}

void StartClient(const SeabattleField& field, const std::string& ip_str, unsigned short port) {
  SeabattleAgent agent(field);

  boost::system::error_code ec;
  auto endpoint = tcp::endpoint(net::ip::make_address(ip_str, ec), port);

  if (ec) {
    std::cout << "Wrong IP format"sv << std::endl;
    throw std::runtime_error(ec.message());
  }

  net::io_context io_context;
  tcp::socket socket{io_context};
  socket.connect(endpoint, ec);

  if (ec) {
    std::cout << "Can't connect to server"sv << std::endl;
    throw std::runtime_error(ec.message());
  }
  agent.StartGame(socket, true);
}

int main(int argc, const char** argv) {
  if (argc != 3 && argc != 4) {
    std::cout << "Usage: program <seed> [<ip>] <port>" << std::endl;
    return 1;
  }

  std::mt19937 engine(std::stoi(argv[1]));
  SeabattleField fieldL = SeabattleField::GetRandomField(engine);

  if (argc == 3) {
    StartServer(fieldL, std::stoi(argv[2]));
  } else if (argc == 4) {
    StartClient(fieldL, argv[2], std::stoi(argv[3]));
  }
}
