#include "doctest.h"

#include "engine/sandbox/sandbox.hh"

TEST_SUITE("Engine")
{
    Sandbox sandbox;
    Board& board = *sandbox.editor().boards().begin();

    TEST_CASE("Basic circuit")
    {
        board.add_component("button", 1, 1);
        board.add_component("led", 3, 1);
        board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 1, 1, 3, 1, Orientation::Horizontal);

        SUBCASE("Board elements")
        {
            CHECK(board.components().size() == 2);
            CHECK(board.components().at({ 1, 1, Direction::Center }).def->name == "button");
            CHECK(board.components().at({ 3, 1, Direction::Center }).def->name == "led");

            CHECK(board.wires().size() == 4);
            CHECK(board.wires().contains({ 1, 1, Direction::E }));
            CHECK(board.wires().contains({ 2, 1, Direction::W }));
            CHECK(board.wires().contains({ 2, 1, Direction::E }));
            CHECK(board.wires().contains({ 3, 1, Direction::W }));
        }
    }
}