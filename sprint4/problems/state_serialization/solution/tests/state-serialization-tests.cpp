#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <catch2/catch_test_macros.hpp>
#include <sstream>

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

SCENARIO_METHOD(Fixture, "LootType serialization") {
    GIVEN("A LootType") {

        const model::LootType loot{
          .name{"key"},
          .file{"assets/key.obj"},
          .type{"obj"},
          .rotation{std::nullopt},
          .color{"#338844"},
          .scale{0.03},
          .value{0},
          .type_num{1}
        };

        WHEN("LootType is serialized") {
            output_archive << loot;

            THEN("it is equal to point after serialization") {
                InputArchive input_archive{strm};
                model::LootType restored_loot;
                input_archive >> restored_loot;
                CHECK(loot == restored_loot);
            }
        }
    }
}
