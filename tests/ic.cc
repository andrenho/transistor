#include <iostream>

#include "doctest.h"

#include "engine/connections/connection.hh"
#include "engine/layout/layout.hh"
#include "engine/sandbox/compiler.hh"
#include "engine/sandbox/sandbox.hh"

TEST_SUITE("ICs")
{
    Sandbox i_sandbox;
    Board& i_board = *i_sandbox.boards().begin();
    ComponentDefinition const& or_2i_def = **i_sandbox.component_db().component_def("or_2i");

    TEST_CASE("IC placement")
    {
        SUBCASE("Don't place ICs over single-tile components")
        {
            i_board.clear();
            i_board.add_component("button", 2, 2, Direction::N);
            CHECK(i_board.add_component("or_2i", 1, 1, Direction::N) == std::optional<Component*>{});
        }

        SUBCASE("Don't place ICs over other IC components")
        {
            i_board.clear();
            i_board.add_component("or_2i", 1, 1, Direction::N);
            CHECK(i_board.add_component("or_2i", 3, 3, Direction::N) == std::optional<Component*>{});
        }

        SUBCASE("Don't place single-tile components over IC components")
        {
            i_board.clear();
            i_board.add_component("or_2i", 1, 1, Direction::N);
            CHECK(i_board.add_component("button", 2, 2, Direction::N)  == std::optional<Component*>{});
        }

        SUBCASE("Overwrite wires")
        {
            i_board.clear();
            i_board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 0, 1, 4, 1, Orientation::Horizontal);
            i_board.add_component("or_2i", 2, 1, Direction::N);

            CHECK(i_board.wires().size() == 4);
            CHECK(i_board.wires().contains({ i_board.id(), 0, 1, Direction::E }));
            CHECK(i_board.wires().contains({ i_board.id(), 1, 1, Direction::W }));
            CHECK(i_board.wires().contains({ i_board.id(), 3, 1, Direction::E }));
            CHECK(i_board.wires().contains({ i_board.id(), 4, 1, Direction::W }));
        }

        SUBCASE("Remove IC")
        {
            i_board.clear();
            i_board.add_component("or_2i", 2, 1, Direction::N);
            CHECK(i_board.components().size() == 1);

            i_board.clear_tile(3, 2);
            CHECK(i_board.components().empty());
        }
    }

    TEST_CASE("Compilation")
    {
        i_board.clear();

        Component* button = *i_board.add_component("button", 0, 1, Direction::N);
        Component* led = *i_board.add_component("led", 4, 1, Direction::N);
        i_board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 0, 1, 4, 1, Orientation::Horizontal);
        Component* or_2i = *i_board.add_component("or_2i", 2, 1, Direction::N);

        Layout layout = compiler::compile_to_layout(i_board);
        Connections connections = compiler::compile_to_connections(std::vector { layout });

        SUBCASE("Board elements")
        {
            CHECK(i_board.components().size() == 3);
            CHECK(&i_board.components().at({ i_board.id(), 0, 1, Direction::Center }) == button);
            CHECK(&i_board.components().at({ i_board.id(), 4, 1, Direction::Center }) == led);
            CHECK(&i_board.components().at({ i_board.id(), 2, 1, Direction::Center }) == or_2i);

            CHECK(i_board.wires().size() == 4);
            CHECK(i_board.wires().contains({ i_board.id(), 0, 1, Direction::E }));
            CHECK(i_board.wires().contains({ i_board.id(), 1, 1, Direction::W }));
            CHECK(i_board.wires().contains({ i_board.id(), 3, 1, Direction::E }));
            CHECK(i_board.wires().contains({ i_board.id(), 4, 1, Direction::W }));
        }

        SUBCASE("Compile editor to layout")
        {
            CHECK(layout.pins.size() == 12);

            CHECK(layout.pins.at({ i_board.id(), 0, 1, Direction::W }).component == button);
            CHECK(layout.pins.at({ i_board.id(), 0, 1, Direction::W }).pin_no == 1);

            CHECK(layout.pins.at({ i_board.id(), 4, 1, Direction::E }).component == led);
            CHECK(layout.pins.at({ i_board.id(), 4, 1, Direction::E }).pin_no == 3);

            CHECK(layout.pins.at({ i_board.id(), 1, 1 }).component == or_2i);
            CHECK(layout.pins.at({ i_board.id(), 1, 1 }).pin_no == 0);
            CHECK(layout.pins.at({ i_board.id(), 1, 2 }).component == or_2i);
            CHECK(layout.pins.at({ i_board.id(), 1, 2 }).pin_no == 1);
            CHECK(layout.pins.at({ i_board.id(), 3, 2 }).component == or_2i);
            CHECK(layout.pins.at({ i_board.id(), 3, 2 }).pin_no == 2);
            CHECK(layout.pins.at({ i_board.id(), 3, 1 }).component == or_2i);
            CHECK(layout.pins.at({ i_board.id(), 3, 1 }).pin_no == 3);

            CHECK(layout.wires.size() == 4);

            CHECK(layout.wires.contains({ i_board.id(), 0, 1, Direction::E }));
            CHECK(layout.wires.contains({ i_board.id(), 1, 1, Direction::W }));
            CHECK(layout.wires.contains({ i_board.id(), 3, 1, Direction::E }));
            CHECK(layout.wires.contains({ i_board.id(), 4, 1, Direction::W }));
        }

        SUBCASE("Compile layout to connections")
        {
            CHECK(connections.size() == 2);

            Connection const& button_connection = *r::find_if(connections, [&](Connection const& c) {
                auto it = r::find_if(c.pins, [&](Pin const& pin) { return pin.component == button; });
                return it != c.pins.end();
            });

            CHECK(button_connection.pins.size() == 2);
            CHECK(r::find_if(button_connection.pins, [&](Pin const& pin) {
                return pin.component == button && pin.pin_no == 3;
            }) != button_connection.pins.end());
            CHECK(r::find_if(button_connection.pins, [&](Pin const& pin) {
                return pin.component == or_2i && pin.pin_no == 0;
            }) != button_connection.pins.end());

            CHECK(button_connection.wires.size() == 2);

            CHECK(r::contains(button_connection.wires, Position { i_board.id(), 0, 1, Direction::E }));
            CHECK(r::contains(button_connection.wires, Position { i_board.id(), 1, 1, Direction::W }));

            Connection const& led_connection = *r::find_if(connections, [&](Connection const& c) {
                auto it = r::find_if(c.pins, [&](Pin const& pin) { return pin.component == led; });
                return it != c.pins.end();
            });

            CHECK(led_connection.pins.size() == 2);
            CHECK(r::find_if(led_connection.pins, [&](Pin const& pin) {
                return pin.component == led && pin.pin_no == 1;
            }) != led_connection.pins.end());
            CHECK(r::find_if(led_connection.pins, [&](Pin const& pin) {
                return pin.component == or_2i && pin.pin_no == 3;
            }) != led_connection.pins.end());

            CHECK(led_connection.wires.size() == 2);

            CHECK(r::contains(led_connection.wires, Position { i_board.id(), 3, 1, Direction::E }));
            CHECK(r::contains(led_connection.wires, Position { i_board.id(), 4, 1, Direction::W }));
        }

        SUBCASE("Simulate without button press")
        {
            button->data[0] = 0;
            i_sandbox.recompile();

            for (size_t i = 0; i < 10; ++i)
                i_sandbox.simulate();
            CHECK(led->data[0] == 0);

            CHECK(i_sandbox.wire_value({ i_board.id(), 2, 1, Direction::W }) == 0);
        }

        SUBCASE("Simulate with button press")
        {
            button->data[0] = 0;
            i_sandbox.recompile();

            button->on_click();
            CHECK(button->data[0] == 1);

            for (size_t i = 0; i < 10; ++i)
                i_sandbox.simulate();
            CHECK(led->data[0] == 1);

            CHECK(i_sandbox.wire_value({ i_board.id(), 4, 1, Direction::W }) == 1);
        }

        SUBCASE("Button press and release")
        {
            button->data[0] = 0;
            i_sandbox.recompile();

            // press

            button->on_click();
            CHECK(button->data[0] == 1);

            for (size_t i = 0; i < 10; ++i)
                i_sandbox.simulate();
            CHECK(led->data[0] == 1);

            // release

            button->on_click();
            CHECK(button->data[0] == 0);

            for (size_t i = 0; i < 10; ++i)
                i_sandbox.simulate();
            CHECK(led->data[0] == 0);

            CHECK(i_sandbox.wire_value({ i_board.id(), 2, 1, Direction::W }) == 0);
        }

        SUBCASE("Serialization and deserialization")
        {
            json content = i_sandbox.serialize();
            std::cout << content.dump(2) << "\n";
            Sandbox sandbox2(content);
            CHECK(i_sandbox == sandbox2);
        }
    }

}