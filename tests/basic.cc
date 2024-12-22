#include "doctest.h"

#include "engine/layout/layout.hh"
#include "engine/sandbox/compiler.hh"
#include "engine/sandbox/sandbox.hh"

TEST_SUITE("Engine")
{
    Sandbox sandbox;
    Board& board = *sandbox.editor().boards().begin();

    ComponentDefinition const& button = sandbox.component_db().component_def("button");
    ComponentDefinition const& led = sandbox.component_db().component_def("led");

    TEST_CASE("Basic circuit")
    {
        board.add_component("button", 1, 1);
        board.add_component("led", 3, 1);
        board.draw_wire(Wire::Width::W1, Wire::Layer::Top, 1, 1, 3, 1, Orientation::Horizontal);

        SUBCASE("Board elements")
        {
            CHECK(board.components().size() == 2);
            CHECK(board.components().at({ 1, 1, Direction::Center }).def == &button);
            CHECK(board.components().at({ 3, 1, Direction::Center }).def == &led);

            CHECK(board.wires().size() == 4);
            CHECK(board.wires().contains({ 1, 1, Direction::E }));
            CHECK(board.wires().contains({ 2, 1, Direction::W }));
            CHECK(board.wires().contains({ 2, 1, Direction::E }));
            CHECK(board.wires().contains({ 3, 1, Direction::W }));
        }

        SUBCASE("Compile to layout")
        {
            Layout layout = compiler::compile_to_layout(board);

            CHECK(layout.pins.size() == 8);

            CHECK(layout.pins.at({ 1, 1, Direction::N }).component->def == &button);
            CHECK(layout.pins.at({ 1, 1, Direction::N }).pin_no == 0);
            CHECK(layout.pins.at({ 1, 1, Direction::W }).component->def == &button);
            CHECK(layout.pins.at({ 1, 1, Direction::W }).pin_no == 1);
            CHECK(layout.pins.at({ 1, 1, Direction::S }).component->def == &button);
            CHECK(layout.pins.at({ 1, 1, Direction::S }).pin_no == 2);
            CHECK(layout.pins.at({ 1, 1, Direction::E }).component->def == &button);
            CHECK(layout.pins.at({ 1, 1, Direction::E }).pin_no == 3);

            CHECK(layout.pins.at({ 3, 1, Direction::N }).component->def == &led);
            CHECK(layout.pins.at({ 3, 1, Direction::N }).pin_no == 0);
            CHECK(layout.pins.at({ 3, 1, Direction::W }).component->def == &led);
            CHECK(layout.pins.at({ 3, 1, Direction::W }).pin_no == 1);
            CHECK(layout.pins.at({ 3, 1, Direction::S }).component->def == &led);
            CHECK(layout.pins.at({ 3, 1, Direction::S }).pin_no == 2);
            CHECK(layout.pins.at({ 3, 1, Direction::E }).component->def == &led);
            CHECK(layout.pins.at({ 3, 1, Direction::E }).pin_no == 3);

            CHECK(layout.wires.size() == 4);

            CHECK(layout.wires.contains({ 1, 1, Direction::E }));
            CHECK(layout.wires.contains({ 2, 1, Direction::W }));
            CHECK(layout.wires.contains({ 2, 1, Direction::E }));
            CHECK(layout.wires.contains({ 3, 1, Direction::W }));
        }
    }
}