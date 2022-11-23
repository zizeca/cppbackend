#ifndef __CONTENT_TYPE_H__
#define __CONTENT_TYPE_H__

#include <string_view>
#include <unordered_map>

namespace http_handler {
using namespace std::string_view_literals;

struct ContentType {
  ContentType() = delete;
  constexpr static std::string_view TEXT_HTML = "text/html"sv;
  constexpr static std::string_view APP_JSON = "application/json"sv;
  static const std::unordered_map<std::string_view, std::string_view> DICT;
};


const std::unordered_map<std::string_view, std::string_view> ContentType::DICT = {
    {".htm", "text/html"},
    {".html", "text/html"},
    {"html", "text/html"},
    {".css", "text/css"},
    {".txt", "text/plain"},
    {".js", "text/javascript"},
    {".json", "application/json"},
    {"json", "application/json"},  
    {".xml", "application/xml"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".jpe", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".gif", "image/gif"},
    {".bmp", "image/bmp"},
    {".ico", "image/vnd.microsoft.icon"},
    {".tiff", "image/tiff"},
    {".tif", "image/tiff"},
    {".svg", "image/svg+xml"},
    {".svgz", "image/svg+xml"},
    {".mp3", "audio/mpeg"},
};


}  // namespace http_handler
#endif // __CONTENT_TYPE_H__