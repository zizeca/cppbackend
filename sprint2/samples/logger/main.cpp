#include <boost/log/core.hpp>         // для logging::core
#include <boost/log/expressions.hpp>  // для выражения, задающего фильтр
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <string_view>
using namespace std::literals;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void InitBoostLogFilter() {
  logging::core::get()->set_filter(
      logging::trivial::severity >= logging::trivial::info);
}

int main() {
  InitBoostLogFilter();

  logging::add_common_attributes();

  int log_num = 1;

  logging::add_file_log(
      keywords::file_name = "sample_%N.log",
      keywords::format = "[%TimeStamp%]: %Message%",
      keywords::open_mode = std::ios_base::app | std::ios_base::out,
      // ротируем по достижению размера 10 мегабайт
      keywords::rotation_size = 10 * 1024 * 1024,
      // ротируем ежедневно в полдень
      keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(12, 0, 0));

  BOOST_LOG_TRIVIAL(trace) << "Сообщение уровня trace, "sv << log_num++;
  BOOST_LOG_TRIVIAL(debug) << "Сообщение уровня debug, "sv << log_num++;
  BOOST_LOG_TRIVIAL(info) << "Сообщение уровня info, "sv << log_num++;
  BOOST_LOG_TRIVIAL(warning) << "Сообщение уровня warning, "sv << log_num++;
  BOOST_LOG_TRIVIAL(error) << "Сообщение уровня error, "sv << log_num++;
  BOOST_LOG_TRIVIAL(fatal) << "Сообщение уровня fatal, "sv << log_num++;
}