#include "sdk.h"
//
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <thread>

#include "json_loader.h"
#include "request_handler.h"
#include "logger.h"

using namespace std::literals;
namespace net = boost::asio;

namespace {

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



}  // namespace

int main(int argc, const char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: game_server <game-config-json> <dir-to-content>"sv << std::endl;
    return EXIT_FAILURE;
  }

  // initial logger
  Logger::Init();
  
  try {
    // 1. Загружаем карту из файла и построить модель игры
    model::Game game = json_loader::LoadGame(argv[1]);

    // 2. Инициализируем io_context
    const unsigned num_threads = std::thread::hardware_concurrency();
    net::io_context ioc(num_threads);

    // 3. Добавляем асинхронный обработчик сигналов SIGINT и SIGTERM
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const boost::system::error_code& ec, [[maybe_unused]] int signal_number) {
      if (!ec) {
        // std::cout << "Signal "sv << signal_number << " received"sv << std::endl;
        ioc.stop();
      }
    });

    // 4. Создаём обработчик HTTP-запросов и связываем его с моделью игры
    http_handler::RequestHandler handler{game, argv[2]};

    LogRequestHandler<http_handler::RequestHandler> loghandler{handler};

    // 5. Запустить обработчик HTTP-запросов, делегируя их обработчику запросов
    /**/

    const auto address = net::ip::make_address("0.0.0.0");
    constexpr net::ip::port_type port = 8080;

    http_server::ServeHttp(ioc, {address, port}, [&loghandler](auto&& req, auto&& send) {
      loghandler(std::forward<decltype(req)>(req), std::forward<decltype(send)>(send));
    });
    /**/

    // Эта надпись сообщает тестам о том, что сервер запущен и готов обрабатывать запросы
    // std::cout << "Server has started..."sv << std::endl;
    boost::json::value jv_port_address{{"port"s, 8080},{"address"s,"0.0.0.0"s}};
    // BOOST_LOG_TRIVIAL(info) <<  boost::log::add_value(additional_data, jv_port_address) << "server started"sv;
    BOOST_LOG_TRIVIAL(info) <<  boost::log::add_value(additional_data, jv_port_address) << "Server has started..."sv;

    // 6. Запускаем обработку асинхронных операций
    RunWorkers(std::max(1u, num_threads), [&ioc] { ioc.run(); });
  } catch (const std::exception& ex) {
    Logger::LogExit(ex);
    return EXIT_FAILURE;
  }
  Logger::LogExit(0);
  return 0;
}
