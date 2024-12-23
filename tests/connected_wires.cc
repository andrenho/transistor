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

    TEST_CASE("simple wire")
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
}