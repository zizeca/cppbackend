//#define BOOST_JSON_STANDALONE 
#include <boost/json.hpp>


// Этот заголовочный файл надо подключить в одном и только одном .cpp-файле программы
#include <boost/json/src.hpp>
#include <iostream>
#include <string>
#include <string_view>
namespace json = boost::json;
using namespace std::literals;

struct A {
  std::string s = "key";
  std::vector<int> v = {1, 2, 3, 4};
};

struct B {
  std::string s;
  std::vector<A> v;
};

void tag_invoke(json::value_from_tag, json::value& jv, A const& a) { jv = {{a.s, json::value_from(a.v)}}; }

void tag_invoke(json::value_from_tag, json::value& jv, B const& b) { jv = {{b.s, json::value_from(b.v)}}; }

int main() {


  A a1{"A_Key1", {1, 2, 3, 4, 5, 6, 7, 8}};
  A a2{"A_Key2", {2, 3, 4, 5, 6, 7, 8}};
  A a3{"A_Key3", {3, 4, 5, 6, 7, 8}};
  A a4{"A_Key4", {4, 5, 6, 7, 8}};
  B b{"keyB", {a1, a2, a3, a4}};

  auto val = json::value_from(b);
  std::cout << "val =  " << json::serialize(val) << std::endl;

}