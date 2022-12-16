#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "../src/tv.h"
#include "../src/menu.h"
#include "../src/controller.h"

#include <string>
#include <string_view>
#include <optional>
#include <iostream>

using namespace std::literals;

SCENARIO("TV behaviour") {
  GIVEN("A TV") {
    Tv tv;

    SECTION("Initially it is off and doesn't show any channel") {
      CHECK(!tv.IsTurnedOn());
      CHECK(!tv.GetChannel().has_value());
    }

    WHEN("it is turned on first time") {
      tv.TurnOn();

      THEN("it is turned on and shows channel #1") {
        CHECK(tv.IsTurnedOn());
        CHECK(tv.GetChannel() == 1);

        AND_WHEN("it is turned off") {
          tv.TurnOff();

          THEN("it is turned off and doesn't show any channel") {
            CHECK(!tv.IsTurnedOn());
            CHECK(tv.GetChannel() == std::nullopt);
          }
        }
      }
    }
  }

  GIVEN("Menu behaviour") {
    Menu menu( std::cin, std::cout);

  }
}