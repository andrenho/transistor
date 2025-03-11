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

    // run tests on Lua
#define LOAD(name) engine.load(#name, engine_tests_##name##_lua, engine_tests_##name##_lua_sz);
    LOAD(compilation)
    LOAD(componentdb)
    LOAD(connected_wires)
    LOAD(cursor)
    LOAD(pinpositions)
    LOAD(placement)
    LOAD(positions)
    LOAD(serialization)
#undef LOAD

    // run simulation tests
    engine.start();
}
