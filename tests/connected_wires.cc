#include "doctest.h"

#include <unordered_set>
#include <vector>

#include "engine/geometry/position.hh"
#include "engine/sandbox/compiler.hh"

namespace compiler {
    std::vector<std::unordered_set<Position>> find_connected_wires(std::unordered_set<Position> wires);
}

TEST_SUITE("Connected wires")
{
    std::unordered_set<Position> positions;

    TEST_CASE("Simple wire")
    {
        positions.insert({ 1, 1, Direction::E });
        positions.insert({ 2, 1, Direction::W });
        positions.insert({ 2, 1, Direction::E });
        positions.insert({ 3, 1, Direction::W });

        auto groups = compiler::find_connected_wires(positions);
        CHECK(groups.size() == 1);

        auto const& group = groups.at(0);
        CHECK(group.size() == 4);
        CHECK(group.contains({ 1, 1, Direction::E }));
        CHECK(group.contains({ 2, 1, Direction::W }));
        CHECK(group.contains({ 2, 1, Direction::E }));
        CHECK(group.contains({ 3, 1, Direction::W }));
    }

    TEST_CASE("Two separate wires")
    {
        positions.insert({ 1, 1, Direction::E });
        positions.insert({ 2, 1, Direction::W });
        positions.insert({ 2, 1, Direction::E });
        positions.insert({ 3, 1, Direction::W });

        positions.insert({ 1, 2, Direction::E });
        positions.insert({ 2, 2, Direction::W });
        positions.insert({ 2, 2, Direction::E });
        positions.insert({ 3, 2, Direction::W });

        auto groups = compiler::find_connected_wires(positions);
        CHECK(groups.size() == 2);

        auto const& group1 = *r::find_if(groups, [](auto const& g) { return g.contains({ 1, 1, Direction::E }); });
        CHECK(group1.size() == 4);
        CHECK(group1.contains({ 1, 1, Direction::E }));
        CHECK(group1.contains({ 2, 1, Direction::W }));
        CHECK(group1.contains({ 2, 1, Direction::E }));
        CHECK(group1.contains({ 3, 1, Direction::W }));

        auto const& group2 = *r::find_if(groups, [](auto const& g) { return g.contains({ 1, 2, Direction::E }); });
        CHECK(group2.size() == 4);
        CHECK(group2.contains({ 1, 2, Direction::E }));
        CHECK(group2.contains({ 2, 2, Direction::W }));
        CHECK(group2.contains({ 2, 2, Direction::E }));
        CHECK(group2.contains({ 3, 2, Direction::W }));
    }

    TEST_CASE("Crossing wires (E shaped)")
    {
        positions.insert({ 1, 1, Direction::E });
        positions.insert({ 2, 1, Direction::W });
        positions.insert({ 2, 1, Direction::E });
        positions.insert({ 3, 1, Direction::W });

        positions.insert({ 2, 1, Direction::N });
        positions.insert({ 2, 0, Direction::S });
        positions.insert({ 2, 0, Direction::E });
        positions.insert({ 3, 0, Direction::W });

        positions.insert({ 2, 1, Direction::S });
        positions.insert({ 2, 2, Direction::N });
        positions.insert({ 2, 2, Direction::E });
        positions.insert({ 3, 2, Direction::W });

        auto groups = compiler::find_connected_wires(positions);
        CHECK(groups.size() == 1);

        auto const& group = groups.at(0);
        CHECK(group.size() == positions.size());
        for (Position const& p: positions)
            CHECK(group.contains(p));
    }
}