#include "sandbox.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <lauxlib.h>
#include <lualib.h>
#include <pl_log.h>

#include <stb_ds.h>

#include "board/board.h"
#include "util/serialize.h"

//
// initialization
//

static ts_Result ts_sandbox_init_common(ts_Sandbox* sb)
{
    memset(sb, 0, sizeof(ts_Sandbox));
    ts_component_db_init(&sb->component_db, sb);

    sb->L = luaL_newstate();
    luaL_openlibs(sb->L);
    luaL_dostring(sb->L, "local bit = require('bit');\n"
                         "bnot, band, bor, bxor, lshift, rshift, rol, ror = bit.bnot, bit.band, bit.bor, bit.bxor, bit.lshift, bit.rshift, bit.rol, bit.ror");

    ts_simulation_init(&sb->simulation, sb);

    PL_DEBUG("Sandbox initialized");
    return TS_OK;
}

ts_Result ts_sandbox_init(ts_Sandbox* sb)
{
    ts_sandbox_init_common(sb);

    arrpush(sb->boards, (ts_Board) {});
    ts_board_init(&sb->boards[0], sb, 20, 10);

    // ts_add_default_components(&sb->component_db);

    ts_sandbox_start_simulation(sb);
    return TS_OK;
}

ts_Result ts_sandbox_finalize(ts_Sandbox* sb)
{
    ts_sandbox_end_simulation(sb);

    for (int i = 0; i < arrlen(sb->boards); ++i)
        ts_board_finalize(&sb->boards[i]);
    arrfree(sb->boards);

    ts_component_db_finalize(&sb->component_db);

    assert(lua_gettop(sb->L) == 0);
    lua_close(sb->L);

    PL_DEBUG("Sandbox finalized");
    return TS_OK;
}

//
// simulation
//

ts_Result ts_sandbox_end_simulation(ts_Sandbox* sb)
{
    return ts_simulation_end(&sb->simulation);
}

ts_Result ts_sandbox_start_simulation(ts_Sandbox* sb)
{
    return ts_simulation_start(&sb->simulation);
}


//
// serialization
//

ts_Result ts_sandbox_serialize(ts_Sandbox const* sb, int vspace, FILE* f)
{
    SR_INIT("{");
    SR_CONT("  boards = {");
    for (int i = 0; i < arrlen(sb->boards); ++i)
        SR_CALL(ts_board_serialize, &sb->boards[i], 4);
    SR_CONT("  },");
    SR_CONT("  component_db = {");
    SR_CALL(ts_component_db_serialize, &sb->component_db, 4);
    SR_CONT("  },");
    SR_CONT("}");
    return TS_OK;
}

static ts_Result ts_sandbox_unserialize(ts_Sandbox* sb, lua_State* LL)
{
    if (!lua_istable(LL, -1))
        PL_ERROR_RET(TS_DESERIALIZATION_ERROR, "The returned element is not a table");

    // component db

    lua_getfield(LL, -1, "component_db");
    if (!lua_istable(LL, -1))
        PL_ERROR_RET(TS_DESERIALIZATION_ERROR, "Expected a table 'component_db'");
    ts_Result r = ts_component_db_unserialize(&sb->component_db, LL, sb);
    // ts_add_default_components(&sb->component_db);
    if (r != TS_OK)
        return r;
    lua_pop(LL, 1);

    // boards

    lua_getfield(LL, -1, "boards");
    if (!lua_istable(LL, -1))
        PL_ERROR_RET(TS_DESERIALIZATION_ERROR, "Expected a table 'boards'");
    lua_pushnil(LL);
    while (lua_next(LL, -2)) {
        arrpush(sb->boards, (ts_Board) {});
        if ((r = ts_board_unserialize(&arrlast(sb->boards), LL, sb)) != TS_OK)
            return r;
        lua_pop(LL, 1);
    }
    lua_pop(LL, 1);

    assert(lua_gettop(LL) == 1);

    return TS_OK;
}

ts_Result ts_sandbox_unserialize_from_string(ts_Sandbox* sb, const char* str)
{
    ts_Result response = TS_OK;

    ts_sandbox_init_common(sb);
    lua_State* LL = luaL_newstate();
    luaL_openlibs(LL);

    if (luaL_dostring(LL, str))
        PL_ERROR_RET(TS_DESERIALIZATION_ERROR, "Error reported from Lua: %s", lua_tostring(LL, -1));

    if (lua_gettop(LL) != 1)
        PL_ERROR_RET(TS_DESERIALIZATION_ERROR, "A element was not added to the stack.");

    response = ts_sandbox_unserialize(sb, LL);
    lua_pop(LL, 1);

end:
    assert(lua_gettop(LL) == 0);
    lua_close(LL);
    return response;
}

ts_Result ts_sandbox_unserialize_from_file(ts_Sandbox* sb, FILE* f)
{
    char* buffer;
    ssize_t bytes_read = getdelim(&buffer, NULL, '\0', f);
    if (bytes_read < 0)
        return TS_SYSTEM_ERROR;
    ts_Result r = ts_sandbox_unserialize_from_string(sb, buffer);
    free(buffer);
    return r;
}
