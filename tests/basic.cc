#include "doctest.h"

#include "engine/sandbox/sandbox.hh"

TEST_SUITE("Engine")
{
    Sandbox sandbox;
    Board& board = sandbox.editor().boards().at(0);

    TEST_CASE("Basic circuit")
    {
        board.add_component("button", 1, 1);
        // board.add_component("led", 3, 1);
        // board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 1, 1, 3, 1, Orientation::Horizontal);

        SUBCASE("check board")
        {

        }
    }
}