#include "doctest.h"

#include <algorithm>
#include <iostream>

#include "engine/connections/connection.hh"
#include "engine/layout/layout.hh"
#include "engine/sandbox/compiler.hh"
#include "engine/sandbox/sandbox.hh"

TEST_SUITE("Engine")
{
    Sandbox e_sandbox;
    Board& e_board = *e_sandbox.boards().begin();

    ComponentDefinition const& button_def = e_sandbox.component_db().component_def("button");
    ComponentDefinition const& led_def = e_sandbox.component_db().component_def("led");

    TEST_CASE("Basic circuit")
    {
        e_board.clear();

        Component* button = e_board.add_component("button", 1, 1);
        Component* led = e_board.add_component("led", 3, 1);
        e_board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 1, 1, 3, 1, Orientation::Horizontal);

        Layout layout = compiler::compile_to_layout(e_board);
        Connections connections = compiler::compile_to_connections(std::vector { layout });

        SUBCASE("Board elements")
        {
            CHECK(e_board.components().size() == 2);
            CHECK(e_board.components().at({ e_board.id(), 1, 1, Direction::Center }).def == &button_def);
            CHECK(e_board.components().at({ e_board.id(), 3, 1, Direction::Center }).def == &led_def);

            CHECK(e_board.wires().size() == 4);
            CHECK(e_board.wires().contains({ e_board.id(), 1, 1, Direction::E }));
            CHECK(e_board.wires().contains({ e_board.id(), 2, 1, Direction::W }));
            CHECK(e_board.wires().contains({ e_board.id(), 2, 1, Direction::E }));
            CHECK(e_board.wires().contains({ e_board.id(), 3, 1, Direction::W }));
        }

        SUBCASE("Compile editor to layout")
        {
            CHECK(layout.pins.size() == 8);

            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::N }).component->def == &button_def);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::N }).pin_no == 0);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::W }).component->def == &button_def);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::W }).pin_no == 1);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::S }).component->def == &button_def);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::S }).pin_no == 2);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::E }).component->def == &button_def);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::E }).pin_no == 3);

            CHECK(layout.pins.at({ e_board.id(), 3, 1, Direction::N }).component->def == &led_def);
            CHECK(layout.pins.at({ e_board.id(), 3, 1, Direction::N }).pin_no == 0);
            CHECK(layout.pins.at({ e_board.id(), 3, 1, Direction::W }).component->def == &led_def);
            CHECK(layout.pins.at({ e_board.id(), 3, 1, Direction::W }).pin_no == 1);
            CHECK(layout.pins.at({ e_board.id(), 3, 1, Direction::S }).component->def == &led_def);
            CHECK(layout.pins.at({ e_board.id(), 3, 1, Direction::S }).pin_no == 2);
            CHECK(layout.pins.at({ e_board.id(), 3, 1, Direction::E }).component->def == &led_def);
            CHECK(layout.pins.at({ e_board.id(), 3, 1, Direction::E }).pin_no == 3);

            CHECK(layout.wires.size() == 4);

            CHECK(layout.wires.contains({ e_board.id(), 1, 1, Direction::E }));
            CHECK(layout.wires.contains({ e_board.id(), 2, 1, Direction::W }));
            CHECK(layout.wires.contains({ e_board.id(), 2, 1, Direction::E }));
            CHECK(layout.wires.contains({ e_board.id(), 3, 1, Direction::W }));
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

            CHECK(connection.wires.size() == 4);

            CHECK(r::contains(connection.wires, Position { e_board.id(), 1, 1, Direction::E }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 2, 1, Direction::W }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 2, 1, Direction::E }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 3, 1, Direction::W }));
        }

        SUBCASE("Simulate without button press")
        {
            button->data[0] = 0;
            e_sandbox.recompile();

            for (size_t i = 0; i < 10; ++i)
                e_sandbox.simulate();
            CHECK(led->data[0] == 0);

            CHECK(e_sandbox.wire_value({ e_board.id(), 2, 1, Direction::W }) == 0);
        }

        SUBCASE("Simulate with button press")
        {
            button->data[0] = 0;
            e_sandbox.recompile();

            button->on_click();
            CHECK(button->data[0] == 1);

            for (size_t i = 0; i < 10; ++i)
                e_sandbox.simulate();
            CHECK(led->data[0] == 1);

            CHECK(e_sandbox.wire_value({ e_board.id(), 2, 1, Direction::W }) == 1);
        }

        SUBCASE("Button press and release")
        {
            button->data[0] = 0;
            e_sandbox.recompile();

            // press

            button->on_click();
            CHECK(button->data[0] == 1);

            for (size_t i = 0; i < 10; ++i)
                e_sandbox.simulate();
            CHECK(led->data[0] == 1);

            // release

            button->on_click();
            CHECK(button->data[0] == 0);

            for (size_t i = 0; i < 10; ++i)
                e_sandbox.simulate();
            CHECK(led->data[0] == 0);

            CHECK(e_sandbox.wire_value({ e_board.id(), 2, 1, Direction::W }) == 0);
        }

        SUBCASE("Serialization and deserialization")
        {
            json content = e_sandbox.serialize();
            std::cout << content.dump(2) << "\n";
            Sandbox sandbox2(content);
            CHECK(e_sandbox == sandbox2);
        }
    }

    TEST_CASE("Multiple connections to same single-tile component")
    {
        e_board.clear();

        Component* led = e_board.add_component("led", 1, 1);
        e_board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 0, 1, 1, 1, Orientation::Horizontal);
        e_board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 1, 0, 1, 1, Orientation::Vertical);

        Layout layout = compiler::compile_to_layout(e_board);
        Connections connections = compiler::compile_to_connections(std::vector { layout });

        CHECK(connections.size() == 2);
    }

    TEST_CASE("Special circuit case 1")
    {
        e_board.clear();

        Component* button = e_board.add_component("button", 1, 0);
        Component* led1 = e_board.add_component("led", 0, 2);
        Component* led2 = e_board.add_component("led", 1, 3);

        e_board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 1, 0, 1, 3, Orientation::Vertical);
        e_board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 1, 2, 0, 2, Orientation::Horizontal);

        Layout layout = compiler::compile_to_layout(e_board);
        Connections connections = compiler::compile_to_connections(std::vector { layout });

        SUBCASE("Compile layout to connections")
        {
            CHECK(connections.size() == 1);

            Connection const& connection = connections.at(0);

            CHECK(connection.pins.size() == 3);
            CHECK(r::find_if(connection.pins, [&](Pin const& pin) { return pin.component == button && pin.pin_no == 2; }) != connection.pins.end());
            CHECK(r::find_if(connection.pins, [&](Pin const& pin) { return pin.component == led1 && pin.pin_no == 3; }) != connection.pins.end());
            CHECK(r::find_if(connection.pins, [&](Pin const& pin) { return pin.component == led2 && pin.pin_no == 0; }) != connection.pins.end());

            CHECK(connection.wires.size() == 8);

            CHECK(r::contains(connection.wires, Position { e_board.id(), 1, 0, Direction::S }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 1, 1, Direction::N }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 1, 1, Direction::S }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 1, 2, Direction::N }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 1, 2, Direction::S }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 1, 3, Direction::N }));

            CHECK(r::contains(connection.wires, Position { e_board.id(), 0, 2, Direction::E }));
            CHECK(r::contains(connection.wires, Position { e_board.id(), 1, 2, Direction::W }));
        }
    }

    TEST_CASE("Single-tile component rotation")
    {
        e_board.clear();

        Component* npn = e_board.add_component("npn", 1, 1);
        e_board.rotate_component(1, 1);

        Layout layout = compiler::compile_to_layout(e_board);

        SUBCASE("Rotate")
        {
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::E }).component == npn);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::E }).pin_no == 0);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::N }).component == npn);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::N }).pin_no == 1);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::W }).component == npn);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::W }).pin_no == 2);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::S }).component == npn);
            CHECK(layout.pins.at({ e_board.id(), 1, 1, Direction::S }).pin_no == 3);
        }
    }
}
