#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <boost/date_time.hpp>
#include <boost/json.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>

BOOST_LOG_ATTRIBUTE_KEYWORD(additional_data, "AdditionalData", boost::json::value);
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime);

using namespace std::literals;

class Logger final {
  Logger() = delete;

 public:
  enum class Where {
    READ,
    WRITE,
    ACCEPT,
  };

  static void LogExit(const int& code);
  static void LogExit(const std::exception& ex);
  static void LogErr(const boost::system::error_code& ec, const std::string& where);
  static void LogErr(const boost::system::error_code& ec, const Where& where);

  static void Init();

 private:
  static void LogFormatter(boost::log::record_view const& rec, boost::log::formatting_ostream& strm);
};

/*
template <class RequestHandler>
class LogRequestHandler {
  static void LogRequest(const Request& r);
  // static void LogResponse(const Response& r);

 public:
  template <typename Body, typename Allocator, typename Send>
  void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
    LogRequest(req);
    decorated_(std::move(req), std::move(send));
    // LogResponse(resp);
    return;
  }

 private:
  SomeRequestHandler& decorated_;
};
 */
#endif  // __LOGGER_H__