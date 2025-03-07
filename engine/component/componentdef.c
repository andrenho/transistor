#include "componentdef.h"

#include <assert.h>
#include <lauxlib.h>
#include <pl_log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sandbox/sandbox.h"

#define TYPE_ERR_MSG "Expected a field 'type' with either: 'single_tile', 'ic_dip' or 'ic_quad'"
#define WIRE_WIDTH_ERR_MSG "Expected a field 'wire_width' with either 1 or 8."

ts_Result ts_component_def_init_from_lua(ts_ComponentDef* def, const char* lua_code, ts_Sandbox* sb, int graphics_luaref)
{
#define ERROR(msg)               { PL_ERROR_RET(TS_COMPONENT_DEF_ERROR, msg); goto end; }
#define EXPECT(type, msg)        { if (!lua_is ## type(L, -1)) ERROR(msg) }
#define EXPECT_OR_NIL(type, msg) { if (!lua_is ## type(L, -1) && !lua_isnil(L, -1)) ERROR(msg) }
#define CHECK_FUNCTION(name)     { lua_getfield(L, -1, name); EXPECT_OR_NIL(function, "'" # name "' should be a function"); lua_pop(L, 1); }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=format"

    PL_DEBUG("Creating component definition from lua code.");

    lua_State* L = sb->L;

    if (luaL_loadstring(L, lua_code) != LUA_OK)
        PL_ERROR_RET(TS_LUA_FUNCTION_ERROR, "syntax error loading component def: %s", lua_tostring(L, -1));
    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
        PL_ERROR_RET(TS_LUA_FUNCTION_ERROR, "syntax error loading component def: %s", lua_tostring(L, -1));

    ts_Result r = TS_OK;
    int initial_stack = lua_gettop(L);

    memset(def, 0, sizeof(ts_ComponentDef));
    EXPECT(table, "Component definition should be a Lua table");

    def->sandbox = sb;

    // key
    assert(lua_gettop(L) == 1);
    lua_getfield(L, -1, "key");
    EXPECT(string, "Expected a field 'key' with the component name.");
    def->key = strdup(lua_tostring(L, -1));
    lua_pop(L, 1);
    PL_DEBUG("  Component def name: %s", def->key);

    // type
    assert(lua_gettop(L) == 1);
    lua_getfield(L, -1, "type");
    EXPECT(string, TYPE_ERR_MSG);
    const char* type = lua_tostring(L, -1);
    if (strcmp(type, "single_tile") == 0)
        def->type = TS_SINGLE_TILE;
    else if (strcmp(type, "ic_dip") == 0)
        def->type = TS_IC_DIP;
    else if (strcmp(type, "ic_quad") == 0)
        def->type = TS_IC_QUAD;
    else
        ERROR(TYPE_ERR_MSG);
    lua_pop(L, 1);

    // can_rotate
    assert(lua_gettop(L) == 1);
    lua_getfield(L, -1, "can_rotate");
    EXPECT_OR_NIL(boolean, "Expected a boolean field 'can_rotate' (optional)");
    def->can_rotate = lua_toboolean(L, -1);
    lua_pop(L, 1);

    // data_size
    assert(lua_gettop(L) == 1);
    lua_getfield(L, -1, "data_size");
    EXPECT_OR_NIL(number, "Expected a numeric field 'data_size' (optional)");
    def->data_size = lua_tonumber(L, -1);
    lua_pop(L, 1);

    // ic_width
    assert(lua_gettop(L) == 1);
    lua_getfield(L, -1, "ic_width");
    EXPECT_OR_NIL(number, "Expected a numeric field 'ic_width' (optional)");
    def->ic_width = lua_isnil(L, -1) ? 1 : lua_tonumber(L, -1);
    lua_pop(L, 1);

    // pins
    assert(lua_gettop(L) == 1);
    lua_getfield(L, -1, "pins");
    EXPECT(table, "Expected a field 'pin', with a list of pins { name, type, wire_width }.");
    def->n_pins = lua_objlen(L, -1);
    def->pins = calloc(def->n_pins, sizeof(ts_PinDef));
    for (size_t i = 0; i < def->n_pins; ++i) {

        lua_rawgeti(L, -1, i + 1);

        // key
        lua_getfield(L, -1, "name");
        EXPECT(string, "Expected a field 'name' with the pin name.");
        def->pins[i].name = strdup(lua_tostring(L, -1));
        lua_pop(L, 1);

        // type
        lua_getfield(L, -1, "direction");
        EXPECT(string, TYPE_ERR_MSG);
        const char* direction = lua_tostring(L, -1);
        if (strcmp(direction, "input") == 0)
            def->pins[i].direction = TS_INPUT;
        else if (strcmp(direction, "output") == 0)
            def->pins[i].direction = TS_OUTPUT;
        else
            ERROR("Expected a field 'direction', with values either 'input' or 'output'.");
        lua_pop(L, 1);

        // wire_width
        lua_getfield(L, -1, "ic_width");
        EXPECT_OR_NIL(number, WIRE_WIDTH_ERR_MSG);
        if (lua_isnil(L, -1) || lua_tonumber(L, -1) == 1)
            def->pins[i].wire_width = TS_WIRE_1;
        else if (lua_tonumber(L, -1) == 8)
            def->pins[i].wire_width = TS_WIRE_8;
        else
            ERROR(WIRE_WIDTH_ERR_MSG);
        lua_pop(L, 2);

    }
    lua_pop(L, 1);

    // check functions
    assert(lua_gettop(L) == 1);
    CHECK_FUNCTION("init")
    CHECK_FUNCTION("init_component")
    CHECK_FUNCTION("on_click")
    CHECK_FUNCTION("simulate")
    CHECK_FUNCTION("render")

    // add own code to table (for serialization)
    assert(lua_gettop(L) == 1);
    lua_pushstring(L, lua_code);
    lua_setfield(L, -2, "__code");

    // store Lua reference
    assert(lua_gettop(L) == 1);
    lua_pushvalue(L, -1);
    def->luaref = luaL_ref(L, LUA_REGISTRYINDEX);
    PL_DEBUG("  Component def reference key: %d", def->luaref);

    // call init, if present
    assert(lua_gettop(L) == 1);
    if (graphics_luaref != -1) {
        lua_getfield(L, -1, "init");
        if (!lua_isnil(L, -1)) {
            lua_rawgeti(L, -1, graphics_luaref);
            PL_DEBUG("  Calling Lua 'init' for component def '%s'", def->key);
            if (lua_pcall(L, 1, 0, 0) != LUA_OK)
                PL_ERROR_RET(TS_LUA_FUNCTION_ERROR, "Error running lua function 'init': %s", lua_tostring(L, -1));
        } else {
            lua_pop(L, 1);
        }
    }

    lua_pop(L, 1);
    assert(lua_gettop(L) == 0);
end:
    if (lua_gettop(L) >= initial_stack)
        lua_pop(L, initial_stack - lua_gettop(L));
    return r;

#pragma GCC diagnostic pop

#undef ERROR
#undef EXPECT
#undef EXPECT_OR_NIL
#undef CHECK_FUNCTION
}

ts_Result ts_component_def_finalize(ts_ComponentDef* def)
{
    PL_DEBUG("Component def '%s' finalized.", def->key);
    luaL_unref(def->sandbox->L, LUA_REGISTRYINDEX, def->luaref);
    free(def->key);
    for (size_t i = 0; i < def->n_pins; ++i)
        free(def->pins[i].name);
    free(def->pins);
    return TS_OK;
}


ts_Rect ts_component_def_rect(ts_ComponentDef const* def, ts_Position component_pos, ts_Direction dir)
{
    int h;

    switch (def->type) {

        case TS_SINGLE_TILE:
            return (ts_Rect) { component_pos, component_pos };

        case TS_IC_DIP:
            h = def->n_pins / 2;
            if (dir == TS_N || dir == TS_S) {
                return (ts_Rect) {
                    (ts_Position) { component_pos.x - 1, component_pos.y - 1, TS_CENTER },
                    (ts_Position) { component_pos.x + def->ic_width, component_pos.y + h, TS_CENTER },
                };
            } else {
                return (ts_Rect) {
                    (ts_Position) { component_pos.x - 1, component_pos.y - 1, TS_CENTER },
                    (ts_Position) { component_pos.x + h, component_pos.y + def->ic_width, TS_CENTER },
                };
            }

        case TS_IC_QUAD:
            h = def->n_pins / 4;
            return (ts_Rect) {
                (ts_Position) { component_pos.x - 1, component_pos.y - 1, TS_CENTER },
                (ts_Position) { component_pos.x + h, component_pos.y + h, TS_CENTER },
            };

        default:
            abort();
    }
}

static ts_Direction single_tile_next_pin(ts_Direction dir)
{
    switch (dir) {
        case TS_N: return TS_W;
        case TS_W: return TS_S;
        case TS_S: return TS_E;
        case TS_E: return TS_N;
        case TS_CENTER:
        default:
            return TS_CENTER;
    }
}

size_t ts_component_def_pin_positions_single_tile(ts_ComponentDef const* def, ts_Position component_pos, ts_Direction direction, ts_PinPos* pin_pos, size_t max_pin_pos)
{
    ts_Direction dir = direction;
    switch (def->n_pins) {
        case 4:
            for (int i = 0; i < 4; ++i) {
                pin_pos[i] = (ts_PinPos) { .pin_no = i, .pos = { component_pos.x, component_pos.y, dir } };
                dir = single_tile_next_pin(dir);
            }
            return 4;
        case 2:
            pin_pos[0] = (ts_PinPos) { .pin_no = 0, .pos = { component_pos.x, component_pos.y, dir } };
            dir = single_tile_next_pin(single_tile_next_pin(dir));
            pin_pos[1] = (ts_PinPos) { .pin_no = 1, .pos = { component_pos.x, component_pos.y, dir } };
            return 2;
        case 1:
            pin_pos[0] = (ts_PinPos) { .pin_no = 0, .pos = { component_pos.x, component_pos.y, dir } };
            return 1;
        default:
            fprintf(stderr, "Single-tile components with %d pins not supported.", def->n_pins);
            abort();
    }
}

size_t ts_component_def_pin_positions_ic_dip(ts_ComponentDef const* def, ts_Position pos, ts_Direction direction, ts_PinPos* pin_pos, size_t max_pin_pos)
{
    int h = def->n_pins / 2;
    int j = 0;
    uint8_t pin_no = 0;

    switch (direction) {
        case TS_N:
            for (int i = 0; i < h; ++i)
                pin_pos[j++] = (ts_PinPos) { .pos = { pos.x - 1, pos.y + i, TS_CENTER }, .pin_no = pin_no++ };
            for (int i = (h-1); i >= 0; --i)
                pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + def->ic_width, pos.y + i, TS_CENTER }, .pin_no = pin_no++ };
            break;
        case TS_E:
            for (int i = 0; i < h; ++i)
                pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + i, pos.y + def->ic_width, TS_CENTER }, .pin_no = pin_no++ };
            for (int i = (h-1); i >= 0; --i)
                pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + i, pos.y - 1, TS_CENTER }, .pin_no = pin_no++ };
            break;
        case TS_S:
            for (int i = (h-1); i >= 0; --i)
                pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + def->ic_width, pos.y + i, TS_CENTER }, .pin_no = pin_no++ };
            for (int i = 0; i < h; ++i)
                pin_pos[j++] = (ts_PinPos) { .pos = { pos.x - 1, pos.y + i, TS_CENTER }, .pin_no = pin_no++ };
            break;
        case TS_W:
            for (int i = (h-1); i >= 0; --i)
                pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + i, pos.y - 1, TS_CENTER }, .pin_no = pin_no++ };
            for (int i = 0; i < h; ++i)
                pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + i, pos.y + def->ic_width, TS_CENTER }, .pin_no = pin_no++ };
            break;
        case TS_CENTER:
        default:
            abort();
    }

    return def->n_pins;
}

size_t ts_component_def_pin_positions_ic_quad(ts_ComponentDef const* def, ts_Position pos, ts_Direction direction, ts_PinPos* pin_pos, size_t max_pin_pos)
{
    int h = def->n_pins / 4;
    int j = 0;
    uint8_t pin_no = 0;

    ts_Direction dir[4];
    switch (direction) {
        case TS_N: dir[0] = TS_W; dir[1] = TS_S; dir[2] = TS_E; dir[3] = TS_N; break;
        case TS_E: dir[0] = TS_N; dir[1] = TS_W; dir[2] = TS_S; dir[3] = TS_E; break;
        case TS_S: dir[0] = TS_E; dir[1] = TS_N; dir[2] = TS_W; dir[3] = TS_S; break;
        case TS_W: dir[0] = TS_S; dir[1] = TS_E; dir[2] = TS_N; dir[3] = TS_W; break;
        case TS_CENTER:
        default:
            abort();
    }

    for (size_t k = 0; k < 4; ++k) {
        switch (dir[k]) {
            case TS_W:
                for (int i = 0; i < h; ++i)
                    pin_pos[j++] = (ts_PinPos) { .pos = { pos.x - 1, pos.y + i, TS_CENTER }, .pin_no = pin_no++ };
                break;
            case TS_S:
                for (int i = 0; i < h; ++i)
                    pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + i, pos.y + h, TS_CENTER }, .pin_no = pin_no++ };
                break;
            case TS_E:
                for (int i = (h-1); i >= 0; --i)
                    pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + h, pos.y + i, TS_CENTER }, .pin_no = pin_no++ };
                break;
            case TS_N:
                for (int i = (h-1); i >= 0; --i)
                    pin_pos[j++] = (ts_PinPos) { .pos = { pos.x + i, pos.y - 1, TS_CENTER }, .pin_no = pin_no++ };
                break;
            case TS_CENTER:
            default:
                abort();
        }
    }

    return def->n_pins;
}

size_t ts_component_def_pin_positions(ts_ComponentDef const* def, ts_Position component_pos, ts_Direction direction, ts_PinPos* pin_pos, size_t max_pin_pos)
{
    if (max_pin_pos < def->n_pins)
        abort();

    switch (def->type) {
        case TS_SINGLE_TILE:
            return ts_component_def_pin_positions_single_tile(def, component_pos, direction, pin_pos, max_pin_pos);
        case TS_IC_DIP:
            return ts_component_def_pin_positions_ic_dip(def, component_pos, direction, pin_pos, max_pin_pos);
        case TS_IC_QUAD:
            return ts_component_def_pin_positions_ic_quad(def, component_pos, direction, pin_pos, max_pin_pos);
        default:
            abort();
    }
}

ts_Result ts_component_def_serialize(ts_ComponentDef const* def, FILE* f)
{
    lua_State* L = def->sandbox->L;
    lua_rawgeti(L, LUA_REGISTRYINDEX, def->luaref);
    lua_getfield(L, -1, "__code");

    const char* code = lua_tostring(L, -1);
    fputc('\'', f);
    for (const char* c = code; (*c) != '\0'; ++c) {
        if (*c == '\n' || *c == '\r') {
            fputc('\\', f);
            fputc('n', f);
        } else if (*c == '\'' || *c == '\\') {
            fputc('\\', f);
            fputc(*c, f);
        } else {
            fputc(*c, f);
        }
    }
    fputc('\'', f);

    lua_pop(L, 2);
    assert(lua_gettop(L) == 0);
    return TS_OK;
}

ts_Result ts_component_def_unserialize(ts_ComponentDef* def, lua_State* L, ts_Sandbox* sb)
{
    return TS_OK;
}
