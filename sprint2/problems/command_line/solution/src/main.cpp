#include "sdk.h"
//
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <memory>
#include <thread>

#include "application.h"
#include "command_parse.h"
#include "json_loader.h"
#include "logger.h"
#include "request_handler.h"
#include "ticker.h"

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
  /*   if (argc != 3) {
      std::cerr << "Usage: game_server <game-config-json> <dir-to-content>"sv << std::endl;
      return EXIT_FAILURE;
    } */

  Args args;
  try {
    auto args_opt = ParseCommandLine(argc, argv);
    if (!args_opt) {
      return 0;
    }
    args = *args_opt;
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  // initial logger
  Logger::Init();

  try {
    // 2. Инициализируем io_context
    const unsigned num_threads = std::thread::hardware_concurrency();
    net::io_context ioc(num_threads);

    Application app(ioc, args.config_file, args.www_root);
    
    if(args.period){
      app.SetManualCallOfTick(false);
      std::make_shared<util::Ticker>(app.strand, std::chrono::milliseconds(args.period), std::bind(&Application::Update, &app, std::placeholders::_1))->Start();
    }

    // signal handler
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const boost::system::error_code& ec, [[maybe_unused]] int signal_number) {
      if (!ec) {
        ioc.stop();
      }
    });

    // request handler
    std::shared_ptr<http_handler::RequestHandler> handler = std::make_shared<http_handler::RequestHandler>(ioc, app);
    // decorator pattern
    LogRequestHandler<http_handler::RequestHandler> loghandler{*handler};

    const auto address = net::ip::make_address("0.0.0.0");
    constexpr net::ip::port_type port = 8080;

    // run server listener
    http_server::ServeHttp(ioc, {address, port}, [&loghandler](auto&& req, auto&& send) {
      loghandler(std::forward<decltype(req)>(req), std::forward<decltype(send)>(send));
    });

    boost::json::value jv_port_address{{"port"s, 8080}, {"address"s, "0.0.0.0"s}};
    BOOST_LOG_TRIVIAL(info) << boost::log::add_value(additional_data, jv_port_address) << "Server has started..."sv;

    //  run async process
    RunWorkers(std::max(1u, num_threads), [&ioc] { ioc.run(); });
  } catch (const std::exception& ex) {
    Logger::LogExit(ex);
    return EXIT_FAILURE;
  }
  Logger::LogExit(0);
  return 0;
}
