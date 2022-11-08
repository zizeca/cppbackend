/**
 * @file Logger.hpp
 * @author Enver Kulametov (zizu.meridian@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__


#include <chrono>
#include <iostream>
#include <syncstream>

using namespace std::chrono;
using namespace std::literals;

class Logger {
 public:
  explicit Logger(std::string id) : id_(std::move(id)) {}

  void LogMessage(std::string_view message) const {
    std::osyncstream os{std::cout};
    os << id_ << "> ["sv << duration<double>(steady_clock::now() - start_time_).count() << "s] "sv << message
       << std::endl;
  }

 private:
  std::string id_;
  steady_clock::time_point start_time_{steady_clock::now()};
};
#endif  // __LOGGER_H__