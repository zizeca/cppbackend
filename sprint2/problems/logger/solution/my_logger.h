#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

using namespace std::literals;

#define LOG(...) Logger::GetInstance().Log(__VA_ARGS__)

class Logger {
  auto GetTime() const {
    if (manual_ts_) {
      return *manual_ts_;
    }

    return std::chrono::system_clock::now();
  }

  auto GetTimeStamp() const {
    const auto now = GetTime();
    const auto t_c = std::chrono::system_clock::to_time_t(now);
    return std::put_time(std::localtime(&t_c), "%F %T");
  }

  // Для имени файла возьмите дату с форматом "%Y_%m_%d"
  std::string GetFileTimeStamp() const {
    const auto now = GetTime();
    std::stringstream ss;
    const auto t_c = std::chrono::system_clock::to_time_t(now);
    ss << "/var/log/sample_log_";
    ss << std::put_time(std::localtime(&t_c), "%Y_%m_%d");
    ss << ".log";
    return ss.str();
  }

  Logger() { openfile(); }

  Logger(const Logger&) = delete;

  void LogPrint() {
    ofs_ << std::endl;
  }

  template <class T>
  void LogPrint(const T& arg) {
    ofs_ << arg << std::endl;
  }

  template <class T, class... Ts>
  void LogPrint(const T& arg, const Ts&... args) {
    ofs_ << arg << " ";
    LogPrint(args...);
  }

  void openfile() {
    std::lock_guard<std::mutex> lk(mx_);
    if (ofs_.is_open()) {
      ofs_.close();
    }
    ofs_.open(GetFileTimeStamp());
    if(!ofs_.is_open()) {
      throw std::runtime_error("fail to open/create log file");
    }
  }

 public:
  static Logger& GetInstance() {
    static Logger obj;
    return obj;
  }

  // Выведите в поток все аргументы.
  template <class... Ts>
  void Log(const Ts&... args) {
    std::lock_guard<std::mutex> lk(mx_);
    ofs_ << GetTimeStamp() << ": ";
    LogPrint(args...);
  }

  // Установите manual_ts_. Учтите, что эта операция может выполняться
  // параллельно с выводом в поток, вам нужно предусмотреть
  // синхронизацию.
  void SetTimestamp(std::chrono::system_clock::time_point ts) {
    std::lock_guard<std::mutex> lk(mx_);
    manual_ts_ = ts;
    openfile();
  }

 private:
  std::optional<std::chrono::system_clock::time_point> manual_ts_;
  std::mutex mx_;
  std::ofstream ofs_;
  std::string path_;
};
