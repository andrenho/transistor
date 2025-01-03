#include "doctest.h"

#include <algorithm>

#include "engine/connections/connection.hh"
#include "engine/layout/layout.hh"
#include "engine/sandbox/compiler.hh"
#include "engine/sandbox/sandbox.hh"

TEST_SUITE("Engine")
{
    Sandbox sandbox;
    Board& board = *sandbox.editor().boards().begin();

    ComponentDefinition const& button_def = sandbox.component_db().component_def("button");
    ComponentDefinition const& led_def = sandbox.component_db().component_def("led");

    TEST_CASE("Basic circuit")
    {
        Component* button = board.add_component("button", 1, 1);
        Component* led = board.add_component("led", 3, 1);
        board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 1, 1, 3, 1, Orientation::Horizontal);

        Layout layout = compiler::compile_to_layout(board);
        Connections connections = compiler::compile_to_connections(std::vector { layout });

        SUBCASE("Board elements")
        {
            CHECK(board.components().size() == 2);
            CHECK(board.components().at({ board.id(), 1, 1, Direction::Center }).def == &button_def);
            CHECK(board.components().at({ board.id(), 3, 1, Direction::Center }).def == &led_def);

            CHECK(board.wires().size() == 4);
            CHECK(board.wires().contains({ board.id(), 1, 1, Direction::E }));
            CHECK(board.wires().contains({ board.id(), 2, 1, Direction::W }));
            CHECK(board.wires().contains({ board.id(), 2, 1, Direction::E }));
            CHECK(board.wires().contains({ board.id(), 3, 1, Direction::W }));
        }

        SUBCASE("Compile editor to layout")
        {
            CHECK(layout.pins.size() == 8);

            CHECK(layout.pins.at({ board.id(), 1, 1, Direction::N }).component->def == &button_def);
            CHECK(layout.pins.at({ board.id(), 1, 1, Direction::N }).pin_no == 0);
            CHECK(layout.pins.at({ board.id(), 1, 1, Direction::W }).component->def == &button_def);
            CHECK(layout.pins.at({ board.id(), 1, 1, Direction::W }).pin_no == 1);
            CHECK(layout.pins.at({ board.id(), 1, 1, Direction::S }).component->def == &button_def);
            CHECK(layout.pins.at({ board.id(), 1, 1, Direction::S }).pin_no == 2);
            CHECK(layout.pins.at({ board.id(), 1, 1, Direction::E }).component->def == &button_def);
            CHECK(layout.pins.at({ board.id(), 1, 1, Direction::E }).pin_no == 3);

            CHECK(layout.pins.at({ board.id(), 3, 1, Direction::N }).component->def == &led_def);
            CHECK(layout.pins.at({ board.id(), 3, 1, Direction::N }).pin_no == 0);
            CHECK(layout.pins.at({ board.id(), 3, 1, Direction::W }).component->def == &led_def);
            CHECK(layout.pins.at({ board.id(), 3, 1, Direction::W }).pin_no == 1);
            CHECK(layout.pins.at({ board.id(), 3, 1, Direction::S }).component->def == &led_def);
            CHECK(layout.pins.at({ board.id(), 3, 1, Direction::S }).pin_no == 2);
            CHECK(layout.pins.at({ board.id(), 3, 1, Direction::E }).component->def == &led_def);
            CHECK(layout.pins.at({ board.id(), 3, 1, Direction::E }).pin_no == 3);

            CHECK(layout.wires.size() == 4);

            CHECK(layout.wires.contains({ board.id(), 1, 1, Direction::E }));
            CHECK(layout.wires.contains({ board.id(), 2, 1, Direction::W }));
            CHECK(layout.wires.contains({ board.id(), 2, 1, Direction::E }));
            CHECK(layout.wires.contains({ board.id(), 3, 1, Direction::W }));
        }

        SUBCASE("Compile layout to connections")
        {
            CHECK(connections.size() == 1);

            Connection const& connection = connections.at(0);

            CHECK(connection.pins.size() == 2);
            CHECK(r::find_if(connection.pins, [](Pin const& pin) {
                return pin.component->def == &button_def && pin.pin_no == 3;
            }) != connection.pins.end());
            CHECK(r::find_if(connection.pins, [](Pin const& pin) {
                return pin.component->def == &led_def && pin.pin_no == 1;
            }) != connection.pins.end());

            CHECK(connection.wire.size() == 4);

            CHECK(r::contains(connection.wire, Position { board.id(), 1, 1, Direction::E }));
            CHECK(r::contains(connection.wire, Position { board.id(), 2, 1, Direction::W }));
            CHECK(r::contains(connection.wire, Position { board.id(), 2, 1, Direction::E }));
            CHECK(r::contains(connection.wire, Position { board.id(), 3, 1, Direction::W }));
        }

        SUBCASE("Simulate without button press")
        {
            button->data[0] = 0;
            sandbox.reset();

            for (size_t i = 0; i < 10; ++i)
                sandbox.simulate();
            CHECK(led->data[0] == 0);

            CHECK(board.wire_value({ board.id(), 2, 1, Direction::W }) == 0);
        }

        SUBCASE("Simulate with button press")
        {
            button->data[0] = 0;
            sandbox.reset();

            button->on_click();
            CHECK(button->data[0] == 1);

            for (size_t i = 0; i < 10; ++i)
                sandbox.simulate();
            CHECK(led->data[0] == 1);

            CHECK(board.wire_value({ board.id(), 2, 1, Direction::W }) == 1);
        }

        SUBCASE("Button press and release")
        {
            button->data[0] = 0;
            sandbox.reset();

            // press

            button->on_click();
            CHECK(button->data[0] == 1);

            for (size_t i = 0; i < 10; ++i)
                sandbox.simulate();
            CHECK(led->data[0] == 1);

            // release

            button->on_click();
            CHECK(button->data[0] == 0);

            for (size_t i = 0; i < 10; ++i)
                sandbox.simulate();
            CHECK(led->data[0] == 0);

            CHECK(board.wire_value({ board.id(), 2, 1, Direction::W }) == 0);
        }
    }
}