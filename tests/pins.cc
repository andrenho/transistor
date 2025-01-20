#include "doctest.h"

#include "engine/connections/connection.hh"
#include "engine/layout/layout.hh"
#include "engine/sandbox/sandbox.hh"
#include "engine/geometry/position.hh"

TEST_SUITE("Pin positions")
{
    using Type = ComponentDefinition::Type;
    static constexpr InputPinType Input = InputPinType::Input;
    static size_t B = 0;

    ComponentDatabase db;

    static Component new_component(ComponentDatabase& db_, ComponentDefinition::Type type, std::vector<InputPinType> const& types)
    {
        auto rnd = [](){ return std::string(15, rand() % 26 + 'a'); };

        std::vector<ComponentDefinition::ComponentPin> pins;
        for (auto type_: types)
            pins.emplace_back(rnd(), type_);

        ComponentDefinition def {
            .name = rnd(),
            .type = type,
            .pins = pins,
        };
        db_.add(def);
        return db_.create_component(def.name);
    }

    TEST_CASE("Single-tile 1 pin")
    {
        Component component = new_component(db, Type::SingleTile, { Input });

        SUBCASE("Direction N")
        {
            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.size() == 1);
            CHECK(pins.at(0).first == 0);
            CHECK(pins.at(0).second == Position(B, 1, 1, Direction::N));
        }

        SUBCASE("Direction E")
        {
            component.rotation = dir_rotate_component(component.rotation);
            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.at(0).second == Position(B, 1, 1, Direction::E));
        }
    }

    TEST_CASE("Single-tile 2 pin")
    {
        Component component = new_component(db, Type::SingleTile, { Input, Input });

        SUBCASE("Direction N")
        {
            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.size() == 2);
            CHECK(pins.at(0).first == 0);
            CHECK(pins.at(0).second == Position(B, 1, 1, Direction::N));
            CHECK(pins.at(1).first == 1);
            CHECK(pins.at(1).second == Position(B, 1, 1, Direction::S));
        }

        SUBCASE("Direction E")
        {
            component.rotation = dir_rotate_component(component.rotation);
            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.size() == 2);
            CHECK(pins.at(0).second == Position(B, 1, 1, Direction::E));
            CHECK(pins.at(1).second == Position(B, 1, 1, Direction::W));
        }
    }

    TEST_CASE("Single-tile 4 pin")
    {
        Component component = new_component(db, Type::SingleTile, { Input, Input, Input, Input });

        SUBCASE("Direction N")
        {
            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.size() == 4);
            CHECK(pins.at(0).first == 0);
            CHECK(pins.at(0).second == Position(B, 1, 1, Direction::N));
            CHECK(pins.at(1).first == 1);
            CHECK(pins.at(1).second == Position(B, 1, 1, Direction::W));
            CHECK(pins.at(2).first == 2);
            CHECK(pins.at(2).second == Position(B, 1, 1, Direction::S));
            CHECK(pins.at(3).first == 3);
            CHECK(pins.at(3).second == Position(B, 1, 1, Direction::E));
        }

        SUBCASE("Direction E")
        {
            component.rotation = dir_rotate_component(component.rotation);
            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.size() == 4);
            CHECK(pins.at(0).second == Position(B, 1, 1, Direction::E));
            CHECK(pins.at(1).second == Position(B, 1, 1, Direction::N));
            CHECK(pins.at(2).second == Position(B, 1, 1, Direction::W));
            CHECK(pins.at(3).second == Position(B, 1, 1, Direction::S));
        }
    }

    TEST_CASE("DIP component 6 pins")
    {
        Component component = new_component(db, Type::IC_DIP, { Input, Input, Input, Input, Input, Input });

        SUBCASE("Direction N")
        {
            CHECK(component.rotation == Direction::N);

            auto [e0, e1] = component.rect(Position(B, 1, 1));
            CHECK(e0 == Position(B, 0, 0));
            CHECK(e1 == Position(B, 2, 4));

            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.size() == 6);
            CHECK(pins.at(0).first == 0);
            CHECK(pins.at(0).second == Position(B, 0, 1));
            CHECK(pins.at(1).first == 1);
            CHECK(pins.at(1).second == Position(B, 0, 2));
            CHECK(pins.at(2).first == 2);
            CHECK(pins.at(2).second == Position(B, 0, 3));
            CHECK(pins.at(3).first == 3);
            CHECK(pins.at(3).second == Position(B, 2, 3));
            CHECK(pins.at(4).first == 4);
            CHECK(pins.at(4).second == Position(B, 2, 2));
            CHECK(pins.at(5).first == 5);
            CHECK(pins.at(5).second == Position(B, 2, 1));
        }

        SUBCASE("Direction E")
        {
            component.rotation = Direction::E;

            auto [e0, e1] = component.rect(Position(B, 1, 1));
            CHECK(e0 == Position(B, 0, 0));
            CHECK(e1 == Position(B, 4, 2));

            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.size() == 6);
            CHECK(pins.at(0).second == Position(B, 1, 2));
            CHECK(pins.at(1).second == Position(B, 2, 2));
            CHECK(pins.at(2).second == Position(B, 3, 2));
            CHECK(pins.at(3).second == Position(B, 3, 0));
            CHECK(pins.at(4).second == Position(B, 2, 0));
            CHECK(pins.at(5).second == Position(B, 1, 0));
        }

        SUBCASE("Direction S")
        {
            component.rotation = Direction::S;

            auto [e0, e1] = component.rect(Position(B, 1, 1));
            CHECK(e0 == Position(B, 0, 0));
            CHECK(e1 == Position(B, 2, 4));

            auto pins = component.pin_positions(Position(B, 1, 1));

            CHECK(pins.size() == 6);
            CHECK(pins.at(0).second == Position(B, 2, 3));
            CHECK(pins.at(1).second == Position(B, 2, 2));
            CHECK(pins.at(2).second == Position(B, 2, 1));
            CHECK(pins.at(3).second == Position(B, 0, 1));
            CHECK(pins.at(4).second == Position(B, 0, 2));
            CHECK(pins.at(5).second == Position(B, 0, 3));
        }
    }
}
