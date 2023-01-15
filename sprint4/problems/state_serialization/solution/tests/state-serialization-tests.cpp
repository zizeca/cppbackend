#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/list.hpp>
// #define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
// #include <boost/serialization/shared_ptr.hpp>
//#include <boost/serialization/unique_ptr.hpp>


#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <sstream>
#include <ranges>

#include "../src/model.h"
#include "../src/model_serialization.h"


using namespace model;
using namespace std::literals;
namespace {

using InputArchive = boost::archive::text_iarchive;
using OutputArchive = boost::archive::text_oarchive;

struct Fixture {
  std::stringstream strm;
  OutputArchive output_archive{strm};
};

struct GameObjectDerived : public model::GameObject {
  GameObjectDerived() : model::GameObject() {}
  template <typename Archive>
  void serialize(Archive& ar, [[maybe_unused]] const unsigned version) {
    ar& boost::serialization::base_object<model::GameObject>(*this);
  }
};

}  // namespace

SCENARIO_METHOD(Fixture, "Point serialization") {
  GIVEN("A point") {
    const model::Point2d p{10, 20};
    WHEN("point is serialized") {
      output_archive << p;

      THEN("it is equal to point after serialization") {
        InputArchive input_archive{strm};
        model::Point2d restored_point;
        input_archive >> restored_point;
        CHECK(p == restored_point);
      }
    }
  }
}

SCENARIO_METHOD(Fixture, "Loot & LootType serialization") {
  GIVEN("A LootType") {
    const LootType loot_type{
        .name{"key"},
        .file{"assets/key.obj"},
        .type{"obj"},
        .rotation{std::nullopt},
        .color{"#338844"},
        .scale{0.03},
        .value{0},
        .type_num{1}};

    WHEN("LootType is serialized") {
      output_archive << loot_type;

      THEN("it is equal to loot_type after serialization") {
        InputArchive input_archive{strm};
        LootType restored_loot_type;
        input_archive >> restored_loot_type;
        CHECK(loot_type == restored_loot_type);
      }
    }

    AND_GIVEN("A Loot") {
      const Point2d pos{10, 20};
      const Point2d wrong_pos{20, 10};
      const unsigned id = 5;

      const Loot loot(loot_type, pos, id);
      WHEN("Loot is serialised") {
        REQUIRE(loot.GetLootType() == loot_type);
        const Loot* ptr = &loot;
        output_archive << ptr;

        THEN("it is equal to loot after serialization") {
          InputArchive input_archive{strm};
          // Loot *restored_loot = static_cast<Loot*>(::operator new(sizeof(Loot)));
          Loot* restored_loot = nullptr;
          input_archive >> restored_loot;
          REQUIRE(restored_loot != nullptr);
          CHECK(loot == *restored_loot);
          CHECK(pos == restored_loot->GetPosition());
          CHECK(id == restored_loot->GetId());
          if (restored_loot) {
            delete restored_loot;
          }
        }
      }
    }

    AND_GIVEN("List of loot") {
      const size_t count = 10;
      std::list<Loot> loots;
      for (auto i = 0, x = 0, y = 0; i < count; i++, x += 2, y += 3) {
        loots.emplace_back(loot_type, Point2d{x, y}, i);
      }

      WHEN("List Loot is serialised") {
        output_archive << loots;

        THEN("it is equal to every loot after serialization") {
          InputArchive input_archive{strm};
          std::list<Loot> restored_loots;
          input_archive >> restored_loots;
          REQUIRE(restored_loots.size() == count);

          auto it_source = loots.begin();
          auto it_target = restored_loots.begin();

          while (it_source != loots.end()) {
            CHECK(*it_source == *it_target);
            ++it_source;
            ++it_target;
          }
        }
      }
    }
  }
}

SCENARIO_METHOD(Fixture, "GameObjectBase serialization") {
  GIVEN("Derive") {
    const model::Point2d pos{10, 20};
    const double width = 0.5;
    GameObjectDerived gobject;
    gobject.SetPosition(pos);
    gobject.SetWidth(width);
    WHEN("GameObject is serialized") {
      output_archive << gobject;

      THEN("it is equal to GameObject after serialization") {
        InputArchive input_archive{strm};
        GameObjectDerived restored_game_object;
        input_archive >> restored_game_object;
        CHECK(restored_game_object.GetPosition() == gobject.GetPosition());
        CHECK(restored_game_object.GetWidth() == gobject.GetWidth());
      }
    }
  }
}

SCENARIO_METHOD(Fixture, "Dog serialization") {
  GIVEN("A Dog") {
    const LootType loot_type{
        .name{"key"},
        .file{"assets/key.obj"},
        .type{"obj"},
        .rotation{std::nullopt},
        .color{"#338844"},
        .scale{0.03},
        .value{0},
        .type_num{1}};

    
    const Dog::Id dog_id(1);

    WHEN("ID serialized") {
      output_archive << &dog_id;
      
      THEN("Is dog id equal") {
        InputArchive input_archive{strm};
        Dog::Id* restored_id = nullptr;
        // boost::shared_ptr<Dog::Id> restored_id;
        // std::shared_ptr<Dog::Id> restored_id;
        // std::unique_ptr<Dog::Id> restored_id;
        input_archive >> restored_id;
        REQUIRE(restored_id != nullptr);
        CHECK( (*dog_id) == (**restored_id));
        delete restored_id;
      }
    }
    
    // // dog create
    Dog dog(dog_id);
    
    const Token token("123456789012345678901234567890ff");
    dog.SetToken(token);

    // // loot
    const size_t count_loot = 10;
    std::list<Loot> loots;
    dog.SetBagSize(count_loot);
    
    // fill loot
    for (auto i = 0, x = 0, y = 0; i < count_loot; i++, x += 2, y += 3) {
      // create loot & add to list for compare
      auto lt = loot_type;
      lt.value = i+10;
      const Loot& loot = loots.emplace_back(lt, Point2d{x, y}, i);
      
      // add loot to dog
      dog.AddLoot(loot);
    }

    // points
    const int points = 10;
    dog.AddPoints(points);

    // position
    const Point2d pos{10.0, 20.0};
    dog.SetPosition(pos);
    const double width = 0.6;
    dog.SetWidth(width);

    WHEN("Dog serialized") {
      output_archive << &dog;

      THEN("it is equal to Dog after serialization") {
        InputArchive input_archive{strm};
        Dog *restored_dog = nullptr;
        input_archive >> restored_dog;
        CHECK(dog.GetId() == restored_dog->GetId());
        CHECK(dog.GetToken() == restored_dog->GetToken());
        CHECK(dog.GetPoinst() == restored_dog->GetPoinst());
        CHECK(dog.GetPosition() == pos);
        CHECK(dog.GetPosition() == restored_dog->GetPosition());
        CHECK(dog.GetWidth() == width);
        CHECK(dog.GetWidth() == restored_dog->GetWidth());
        REQUIRE(dog.GetLoots().size() == restored_dog->GetLoots().size());
        CHECK(std::equal(dog.GetLoots().cbegin(), dog.GetLoots().cend(), restored_dog->GetLoots().cbegin()));


        //CHECK_THAT(dog.GetLoots(), Catch::Matchers::AllMatch(restored_dog->GetLoots()));

        delete restored_dog;
      }
    }

  }
}