#ifndef __UTIL_H__
#define __UTIL_H__

#include <filesystem>
namespace util {
namespace fs = std::filesystem;

bool IsSubPath(fs::path path, fs::path base);
std::string url_decode(const std::string &s);
};      // namespace util
#endif  // __UTIL_H__