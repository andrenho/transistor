#include "tests.hh"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <thread>

#include "engine.hh"

#include "engine/tests/compilation.lua.h"
#include "engine/tests/componentdb.lua.h"
#include "engine/tests/connected_wires.lua.h"
#include "engine/tests/cursor.lua.h"
#include "engine/tests/pinpositions.lua.h"
#include "engine/tests/placement.lua.h"
#include "engine/tests/positions.lua.h"
#include "engine/tests/serialization.lua.h"

void Tests::run_tests()
{
    printf("=== RUNNING TESTS ===============================================\n");
    test_native_query();
    test_lua_engine();
    test_simulation();
    printf("=== TESTS SUCCESSFUL ============================================\n");
}

void Tests::test_native_query()
{
    Engine engine;

    // test NativeQuery
    engine.execute(R"(
        print('Native query')
        do
            print('- Operations')
            local n = native_array(1)
            n[1] = 48
            assert(n[1] == 48)
            assert(#n == 1)
         end
    )", false);
}

void Tests::test_lua_engine()
{
    Engine engine;

#define LOAD(name) engine.load_bytecode(#name, engine_engine_tests_##name##_lua, engine_engine_tests_##name##_lua_sz);
    LOAD(compilation)
    LOAD(componentdb)
    LOAD(connected_wires)
    LOAD(cursor)
    LOAD(pinpositions)
    LOAD(placement)
    LOAD(positions)
    LOAD(serialization)
#undef LOAD
}

void Tests::test_simulation()
{
    Engine engine;

    engine.start();
    engine.execute(R"(
        print('Simulation')
        print(' - Simulate simple circuit')
        sandbox.boards[1]:add_component('__button', P(1, 1), N)
        sandbox.boards[1]:add_component('__led', P(3, 1), N)
        sandbox.boards[1]:add_wires(P(1, 1), P(3, 1), HORIZONTAL, WR(LAYER_TOP, WIDTH_1))
    )");

    auto find_button = [](Snapshot const& snap) -> Snapshot::Component const* {
        return &*std::find_if(snap.boards.at(0).components.begin(), snap.boards.at(0).components.end(),
            [](Snapshot::Component const& comp) { return comp.key == "__button"; });
    };
    {
        Snapshot snap = engine.take_snapshot();
        assert(find_button(snap)->data.at(0) == 0);
        assert(snap.boards.at(0).wires.at(0).value == 0);
    }

    // click button
    engine.execute("sandbox.boards[1]:component(P(1, 1)):on_click()", false);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    {
        Snapshot snap = engine.take_snapshot();
        assert(find_button(snap)->data.at(0) != 0);
        assert(snap.boards.at(0).wires.at(0).value != 0);
    }

    // click button again
    engine.execute("sandbox.boards[1]:component(P(1, 1)):on_click()", false);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    {
        Snapshot snap = engine.take_snapshot();
        assert(find_button(snap)->data.at(0) == 0);
        assert(snap.boards.at(0).wires.at(0).value == 0);
    }

}
