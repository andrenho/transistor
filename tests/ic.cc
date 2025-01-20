#include "doctest.h"

#include "engine/connections/connection.hh"
#include "engine/layout/layout.hh"
#include "engine/sandbox/sandbox.hh"

TEST_SUITE("ICs")
{
    Sandbox i_sandbox;
    Board& board = *i_sandbox.boards().begin();
    ComponentDefinition const& or_2i = i_sandbox.component_db().component_def("or_2i");

    TEST_CASE("DIP Pin locations")
    {
    }
}