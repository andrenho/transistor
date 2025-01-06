#include "doctest.h"

#include <iostream>
#include <unordered_set>
#include <vector>

#include "engine/geometry/position.hh"
#include "engine/sandbox/compiler.hh"

namespace compiler {
    std::vector<std::unordered_set<Position>> find_connected_wires(std::unordered_set<Position> wires, std::unordered_set<Position> const& single_tile_component_pins={});
}

TEST_SUITE("Connected wires")
{
    std::unordered_set<Position> positions;
    const size_t BOARD_ID = 0;

    TEST_CASE("Simple wire")
    {
        positions = {};

        positions.insert({ BOARD_ID, 1, 1, Direction::E });
        positions.insert({ BOARD_ID, 2, 1, Direction::W });
        positions.insert({ BOARD_ID, 2, 1, Direction::E });
        positions.insert({ BOARD_ID, 3, 1, Direction::W });

        auto groups = compiler::find_connected_wires(positions);
        CHECK(groups.size() == 1);

        auto const& group = groups.at(0);
        CHECK(group.size() == 4);
        CHECK(group.contains({ BOARD_ID, 1, 1, Direction::E }));
        CHECK(group.contains({ BOARD_ID, 2, 1, Direction::W }));
        CHECK(group.contains({ BOARD_ID, 2, 1, Direction::E }));
        CHECK(group.contains({ BOARD_ID, 3, 1, Direction::W }));
    }

    TEST_CASE("Two separate wires")
    {
        positions = {};

        positions.insert({ BOARD_ID, 1, 1, Direction::E });
        positions.insert({ BOARD_ID, 2, 1, Direction::W });
        positions.insert({ BOARD_ID, 2, 1, Direction::E });
        positions.insert({ BOARD_ID, 3, 1, Direction::W });

        positions.insert({ BOARD_ID, 1, 2, Direction::E });
        positions.insert({ BOARD_ID, 2, 2, Direction::W });
        positions.insert({ BOARD_ID, 2, 2, Direction::E });
        positions.insert({ BOARD_ID, 3, 2, Direction::W });

        auto groups = compiler::find_connected_wires(positions);
        CHECK(groups.size() == 2);

        auto const& group1 = *r::find_if(groups, [](auto const& g) { return g.contains({ BOARD_ID, 1, 1, Direction::E }); });
        CHECK(group1.size() == 4);
        CHECK(group1.contains({ BOARD_ID, 1, 1, Direction::E }));
        CHECK(group1.contains({ BOARD_ID, 2, 1, Direction::W }));
        CHECK(group1.contains({ BOARD_ID, 2, 1, Direction::E }));
        CHECK(group1.contains({ BOARD_ID, 3, 1, Direction::W }));

        auto const& group2 = *r::find_if(groups, [](auto const& g) { return g.contains({ BOARD_ID, 1, 2, Direction::E }); });
        CHECK(group2.size() == 4);
        CHECK(group2.contains({ BOARD_ID, 1, 2, Direction::E }));
        CHECK(group2.contains({ BOARD_ID, 2, 2, Direction::W }));
        CHECK(group2.contains({ BOARD_ID, 2, 2, Direction::E }));
        CHECK(group2.contains({ BOARD_ID, 3, 2, Direction::W }));
    }

    TEST_CASE("Crossing wires (E shaped)")
    {
        positions = {};

        positions.insert({ BOARD_ID, 1, 1, Direction::E });
        positions.insert({ BOARD_ID, 2, 1, Direction::W });
        positions.insert({ BOARD_ID, 2, 1, Direction::E });
        positions.insert({ BOARD_ID, 3, 1, Direction::W });

        positions.insert({ BOARD_ID, 2, 1, Direction::N });
        positions.insert({ BOARD_ID, 2, 0, Direction::S });
        positions.insert({ BOARD_ID, 2, 0, Direction::E });
        positions.insert({ BOARD_ID, 3, 0, Direction::W });

        positions.insert({ BOARD_ID, 2, 1, Direction::S });
        positions.insert({ BOARD_ID, 2, 2, Direction::N });
        positions.insert({ BOARD_ID, 2, 2, Direction::E });
        positions.insert({ BOARD_ID, 3, 2, Direction::W });

        auto groups = compiler::find_connected_wires(positions);
        CHECK(groups.size() == 1);

        auto const& group = groups.at(0);
        CHECK(group.size() == positions.size());
        for (Position const& p: positions)
            CHECK(group.contains(p));
    }

    TEST_CASE("Strange shape")
    {
        positions = {};

        positions.insert({ BOARD_ID, 1, 0, Direction::S });
        positions.insert({ BOARD_ID, 1, 1, Direction::N });
        positions.insert({ BOARD_ID, 1, 1, Direction::S });
        positions.insert({ BOARD_ID, 1, 2, Direction::N });
        positions.insert({ BOARD_ID, 1, 2, Direction::S });
        positions.insert({ BOARD_ID, 1, 3, Direction::N });

        positions.insert({ BOARD_ID, 0, 2, Direction::E });
        positions.insert({ BOARD_ID, 1, 2, Direction::W });

        auto groups = compiler::find_connected_wires(positions);
        CHECK(groups.size() == 1);

        auto const& group = groups.at(0);
        CHECK(group.size() == positions.size());
        for (Position const& p: positions)
            CHECK(group.contains(p));
    }

    TEST_CASE("Single-tile component in middle of connection")
    {
        positions = {};

        positions.insert({ BOARD_ID, 1, 1, Direction::W });
        positions.insert({ BOARD_ID, 1, 1, Direction::S });

        auto groups = compiler::find_connected_wires(positions, { { BOARD_ID, 1, 1 } });

        CHECK(groups.size() == 2);

        CHECK(groups.at(0).size() == 1);
        bool c1 = (*groups.at(0).begin() == Position { BOARD_ID, 1, 1, Direction::W }) || (*groups.at(0).begin() == Position { BOARD_ID, 1, 1, Direction::S });
        CHECK(c1);

        CHECK(groups.at(1).size() == 1);
        bool c2 = (*groups.at(1).begin() == Position { BOARD_ID, 1, 1, Direction::W }) || (*groups.at(1).begin() == Position { BOARD_ID, 1, 1, Direction::S });
        CHECK(c2);

        CHECK(*groups.at(0).begin() != *groups.at(1).begin());
    }
}