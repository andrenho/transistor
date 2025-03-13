#include "tests.hh"

#include "engine.hh"

#include "engine/tests/compilation.lua.h"
#include "engine/tests/componentdb.lua.h"
#include "engine/tests/connected_wires.lua.h"
#include "engine/tests/cursor.lua.h"
#include "engine/tests/pinpositions.lua.h"
#include "engine/tests/placement.lua.h"
#include "engine/tests/positions.lua.h"
#include "engine/tests/serialization.lua.h"

void run_tests()
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

    // run tests on Lua
#define LOAD(name) engine.load_bytecode(#name, engine_tests_##name##_lua, engine_tests_##name##_lua_sz);
    LOAD(compilation)
    LOAD(componentdb)
    LOAD(connected_wires)
    LOAD(cursor)
    LOAD(pinpositions)
    LOAD(placement)
    LOAD(positions)
    LOAD(serialization)
#undef LOAD

    // test simulation

    engine.start();
    engine.execute(R"(
        print('Simulation')
        print(' - Simulate simple circuit')
        sandbox.boards[1]:add_component('__button', P(1, 1), N)
        sandbox.boards[1]:add_component('__led', P(3, 1), N)
        sandbox.boards[1]:add_wires(P(1, 1), P(3, 1), HORIZONTAL, WR(LAYER_TOP, WIDTH_1))
    )");

    Snapshot snap = engine.take_snapshot();

    // TODO - on click
    // TODO - take snapshot

    // TODO - on click again
    // TODO - take snapshot

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}
