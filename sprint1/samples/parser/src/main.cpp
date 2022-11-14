//#define BOOST_JSON_STANDALONE  // for std::string_viev
#include <boost/json.hpp>
// #include <boost/json/parse.hpp>

// Этот заголовочный файл надо подключить в одном и только одном .cpp-файле программы
#include <boost/json/src.hpp>
#include <iostream>
#include <string>
#include <string_view>
namespace json = boost::json;
using namespace std::literals;

#pragma region

std::string_view map = R"({
  "maps": [
    {
      "id": "map1",
      "name": "Map 1",
      "roads": [
        {
          "x0": 0,
          "y0": 0,
          "x1": 40
        },
        {
          "x0": 40,
          "y0": 0,
          "y1": 30
        },
        {
          "x0": 40,
          "y0": 30,
          "x1": 0
        },
        {
          "x0": 0,
          "y0": 0,
          "y1": 30
        }
      ],
      "buildings": [
        {
          "x": 5,
          "y": 5,
          "w": 30,
          "h": 20
        }
      ],
      "offices": [
        {
          "id": "o0",
          "x": 40,
          "y": 30,
          "offsetX": 5,
          "offsetY": 0
        }
      ]
    },
    {
      "id": "map2",
      "name": "Map 2",
      "roads": [
        {
          "x0": 0,
          "y0": 0,
          "x1": 40
        },
        {
          "x0": 40,
          "y0": 0,
          "y1": 30
        }
      ]
    }
  ]
})"sv;

#pragma endregion

struct A {
  std::string s = "key";
  std::vector<int> v = {1, 2, 3, 4};
};

struct B {
  std::string s;
  std::vector<A> v;
  std::vector<A>& get() const { return v; }
};

void tag_invoke(json::value_from_tag, json::value& jv, A const& a) { jv = {{a.s, json::value_from(a.v)}}; }

void tag_invoke(json::value_from_tag, json::value& jv, B const& b) { jv = {{b.s, json::value_from(b.get())}}; }

int main() {
  // std::cout << map << std::endl;
  auto value = json::parse(std::string(map));
  // if(value.is_object()){
  //   std::cout << "value is object " << value.as_object().at("maps"sv) << std::endl;
  // }

  // auto arr = value.as_object().at("maps"sv).as_array();

  auto maps = value.as_object().at("maps").as_array();

  for (auto m = maps.cbegin(); m != maps.end(); ++m) {
    std::cout << "Maps id " << m->at("id").as_string() << " name" << m->at("name").as_string() << std::endl;
    for (auto i = m->as_object().cbegin(); i != m->as_object().cend(); i++) {
      if (i->key() == "id" || i->key() == "name") {
        continue;
      } else if (i->key() == "roads") {
        std::cout << "Road\n";
      } else if (i->key() == "buildings") {
        std::cout << "buildings\n";
      } else if (i->key() == "offices") {
        std::cout << "offices\n";
      } else {
        std::cout << "unknon object " << i->key() << std::endl;
      }
    }
  }

  std::vector<std::string> varr = {"val_1"s, "val_2"s, "val_3"s, "val_4"s};

  auto v1 = json::value_from("val"sv);
  auto v2 = json::value_from(varr);
  std::cout << "v1 " << json::serialize(v1) << std::endl;
  std::cout << "v2 " << json::serialize(v2) << std::endl;

  A a{"anotherKey", {1, 2, 3, 4, 5, 6, 7, 8}};

  auto v3 = json::value_from(a);
  std::cout << "v3 " << json::serialize(v3) << std::endl;

  A a1{"A_Key1", {1, 2, 3, 4, 5, 6, 7, 8}};
  A a2{"A_Key2", {2, 3, 4, 5, 6, 7, 8}};
  A a3{"A_Key3", {3, 4, 5, 6, 7, 8}};
  A a4{"A_Key4", {4, 5, 6, 7, 8}};
  B b{"keyB", {a1, a2, a3, a4}};

  auto v4 = json::value_from(b);
  std::cout << "v4 B " << json::serialize(v4) << std::endl;

  // std::cout << arr->key() << std::endl;

  /*
    int count = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
      std::cout << "Count iteration = " << count++ << std::endl;
      std::cout << "kind " << json::to_string(it->kind()) << std::endl;
      std::cout << it->as_object() << std::endl;

      for (auto i = it->as_object().begin(); i != it->as_object().end(); ++i) {
        std::cout << "key" <<  i->key() << " value" << i->value() << std::endl;
      }
    }
   */
  // std::cout << value.as_object().at("maps"sv).as_array()[0].as_object().at("id"sv) << std::endl;  // Harry Potter
  //  std::cout << json::serialize(value) << std::endl;          // {"name": "Harry Potter"}
}