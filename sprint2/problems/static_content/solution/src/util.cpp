#include "util.h"

namespace util {

bool IsSubPath(fs::path path, fs::path base) {
  // Приводим оба пути к каноничному виду (без . и ..)
  path = fs::weakly_canonical(path);
  base = fs::weakly_canonical(base);

  // Проверяем, что все компоненты base содержатся внутри path
  for (auto b = base.begin(), p = path.begin(); b != base.end(); ++b, ++p) {
    if (p == path.end() || *p != *b) {
      return false;
    }
  }
  return true;
}

std::string url_decode(const std::string &s) {
  std::string str;
  str.resize(s.size());

  for (auto i = 0, j = 0; i < s.size(); ++i, ++j) {
    if (s[i] == '%' && i + 2 < s.size()) {
      std::string r = s.substr(i + 1, 2);
      str[j] = (unsigned char)std::stoul(r, nullptr, 16);
      i += 2;
      continue;
    }
    str[j] = s[i];
  }
  return str;
}

};  // namespace util