#include "logger.h"

void Logger::LogExit(const int& code) {
  boost::json::value jv{{"code", code}};
  if (code)
    BOOST_LOG_TRIVIAL(error) << boost::log::add_value(additional_data, jv) << "server exited"sv;
  else
    BOOST_LOG_TRIVIAL(info) << boost::log::add_value(additional_data, jv) << "server exited"sv;
}

void Logger::LogExit(const std::exception& ex) {
  boost::json::value jv{{"exception", ex.what()}};
  BOOST_LOG_TRIVIAL(error) << boost::log::add_value(additional_data, jv) << "server exited"sv;
}

void Logger::LogErr(const boost::system::error_code& ec, const std::string& where) {
  boost::json::value jv = {
      {"code", ec.value()},
      {"text", ec.message()},
      {"where", where},
  };
  BOOST_LOG_TRIVIAL(error) << boost::log::add_value(additional_data, jv) << "error"sv;
}

void Logger::LogErr(const boost::system::error_code& ec, const Where& where) {
  switch (where) {
    case Where::READ:
      LogErr(ec, "read");
      break;
    case Where::WRITE:
      LogErr(ec, "write");
      break;
    case Where::ACCEPT:
      LogErr(ec, "accept");
      break;
    default:
      throw std::logic_error("switch unknown code");
      break;
  }
}

void Logger::LogFormatter(boost::log::record_view const& rec, boost::log::formatting_ostream& strm) {
  auto ts = *rec[timestamp];
  strm << "{\"timestamp\":\""sv << to_iso_extended_string(ts) << "\","sv;

  auto json_data = boost::json::serialize(*rec[additional_data]);

  strm << "\"data\":"sv << json_data << ","sv;
  strm << "\"message\":\""sv << rec[boost::log::expressions::smessage] << "\"}"sv;
}

void Logger::Init() {
  boost::log::add_common_attributes();
  boost::log::add_console_log(
      std::cout,
      boost::log::keywords::auto_flush = true,
      boost::log::keywords::format = &LogFormatter);
}
