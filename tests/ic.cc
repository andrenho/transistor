#include "doctest.h"

#include "engine/connections/connection.hh"
#include "engine/layout/layout.hh"
#include "engine/sandbox/compiler.hh"
#include "engine/sandbox/sandbox.hh"

TEST_SUITE("ICs")
{
    Sandbox i_sandbox;
    Board& i_board = *i_sandbox.boards().begin();
    ComponentDefinition const& or_2i_def = i_sandbox.component_db().component_def("or_2i");

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

    }
}