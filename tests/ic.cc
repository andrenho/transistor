#include "doctest.h"

#include "engine/connections/connection.hh"
#include "engine/layout/layout.hh"
#include "engine/sandbox/sandbox.hh"

TEST_SUITE("ICs")
{
    Sandbox i_sandbox;
    Board& i_board = *i_sandbox.boards().begin();
    ComponentDefinition const& or_2i = i_sandbox.component_db().component_def("or_2i");

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
    }
}