#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <boost/date_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <string_view>

BOOST_LOG_ATTRIBUTE_KEYWORD(additional_data, "AdditionalData", boost::json::value);
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime);

using namespace std::string_view_literals;


void LogFormatter(boost::log::record_view const& rec, boost::log::formatting_ostream& strm) {
  
  auto ts = *rec[timestamp];
  strm << "{\"timestamp\":\""sv << to_iso_extended_string(ts) << "\","sv;

  auto json_data = boost::json::serialize(*rec[additional_data]); 

  strm << "\"data\":"sv << json_data << ","sv;
  strm << "\"message\":\""sv << rec[boost::log::expressions::smessage] << "\"}"sv ;
}

void InitBoostLogger() {
  boost::log::add_common_attributes();
  boost::log::add_console_log(
    std::cout,
    boost::log::keywords::auto_flush = true, 
    boost::log::keywords::format = &LogFormatter
  );
}
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