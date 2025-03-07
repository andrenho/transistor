#include "board.h"

#include <lauxlib.h>
#include <pl_log.h>
#include <stb_ds.h>

#include "util/serialize.h"
#include "component/componentdb.h"
#include "sandbox/sandbox.h"

ts_HashPosComponentPtr ts_board_component_tiles(ts_Board const* board);

//
// initialization
//

ts_Result ts_board_init(ts_Board* board, ts_Sandbox* sb, int w, int h)
{
    board->sandbox = sb;
    board->w = w;
    board->h = h;
    board->wires = NULL;
    board->components = NULL;
    ts_cursor_init(&board->cursor, board);
    PL_DEBUG("Board created.");
    return TS_OK;
}

ts_Result ts_board_finalize(ts_Board* board)
{
    ts_cursor_finalize(&board->cursor);

    for (int i = 0; i < phlen(board->components); ++i) {
        ts_component_finalize(board->components[i].value);
        free(board->components[i].value);
    }
    phfree(board->components);
    phfree(board->wires);
    PL_DEBUG("Board finalized.");
    return TS_OK;
}

//
// wires
//

static ts_Result ts_board_remove_wires_for_ic(ts_Board* board, ts_Rect rect)
{
    // remove sides
    for (int y = rect.top_left.y; y <= rect.bottom_right.y; ++y) {  // left
        ts_board_remove_wire(board, (ts_Position) { rect.top_left.x, y, TS_N });
        ts_board_remove_wire(board, (ts_Position) { rect.top_left.x, y, TS_S });
        ts_board_remove_wire(board, (ts_Position) { rect.top_left.x, y, TS_E });
    }
    for (int y = rect.top_left.y; y <= rect.bottom_right.y; ++y) {  // right
        ts_board_remove_wire(board, (ts_Position) { rect.bottom_right.x, y, TS_N });
        ts_board_remove_wire(board, (ts_Position) { rect.bottom_right.x, y, TS_S });
        ts_board_remove_wire(board, (ts_Position) { rect.bottom_right.x, y, TS_W });
    }
    for (int x = rect.top_left.x; x <= rect.bottom_right.x; ++x) {  // top
        ts_board_remove_wire(board, (ts_Position) { x, rect.top_left.y, TS_S });
        ts_board_remove_wire(board, (ts_Position) { x, rect.top_left.y, TS_W });
        ts_board_remove_wire(board, (ts_Position) { x, rect.top_left.y, TS_E });
    }
    for (int x = rect.top_left.x; x <= rect.bottom_right.x; ++x) {  // bottom
        ts_board_remove_wire(board, (ts_Position) { x, rect.bottom_right.y, TS_N });
        ts_board_remove_wire(board, (ts_Position) { x, rect.bottom_right.y, TS_W });
        ts_board_remove_wire(board, (ts_Position) { x, rect.bottom_right.y, TS_E });
    }

    // remove center
    for (int x = rect.top_left.x + 1; x < rect.bottom_right.x; ++x) {
        for (int y = rect.top_left.y + 1; y < rect.bottom_right.y; ++y) {
            ts_board_remove_wire(board, (ts_Position) { x, y, TS_N });
            ts_board_remove_wire(board, (ts_Position) { x, y, TS_S });
            ts_board_remove_wire(board, (ts_Position) { x, y, TS_W });
            ts_board_remove_wire(board, (ts_Position) { x, y, TS_E });
        }
    }

    return TS_OK;
}

ts_Result ts_board_remove_wire(ts_Board* board, ts_Position position)
{
    PL_DEBUG("Removing wires at %d,%d", position.x, position.y);
    phdel(board->wires, position);
    return TS_OK;
}

ts_Wire* ts_board_wire(ts_Board const* board, ts_Position pos)
{
    int i = phgeti(((ts_Board *) board)->wires, pos);
    if (i == -1)
        return NULL;
    return &board->wires[i].value;
}

size_t ts_board_wires(ts_Board const* board, ts_Position* positions, uint8_t* data, size_t sz)
{
    return ts_simulation_wires(&board->sandbox->simulation, positions, data, sz);
}

static void ts_remove_wires_under_ics(ts_Board* board)
{
    for (int i = 0; i < phlen(board->components); ++i)
        if (board->components[i].value->def->type != TS_SINGLE_TILE)
            ts_board_remove_wires_for_ic(board, ts_component_rect(board->components[i].value));
}

ts_Result ts_board_add_wire(ts_Board* board, ts_Position pos, ts_Wire wire)
{
    PL_DEBUG("Adding wire at %d,%d", pos.x, pos.y);

    ts_Result r = TS_OK;
    ts_sandbox_end_simulation(board->sandbox);

    // add wire
    if (pos.x < board->w && pos.y < board->h)
        phput(board->wires, pos, wire);
    else
        PL_ERROR_RET(TS_CANNOT_PLACE, "Wire out of bounds");

    ts_remove_wires_under_ics(board);

    ts_sandbox_start_simulation(board->sandbox);
    return r;
}

ts_Result ts_board_add_wires(ts_Board* board, ts_Position start, ts_Position end, ts_Orientation orientation, ts_Wire wire)
{
    PL_DEBUG("Adding wires from %d,%d to %d,%d", start.x, start.y, end.x, end.y);

    if (ts_pos_equals(start, end))
        return TS_OK;

    ts_sandbox_end_simulation(board->sandbox);

    ts_Position pos[300];
    size_t sz = ts_pos_a_to_b(start, end, orientation, pos, 300);
    for (size_t i = 0; i < sz; ++i) {
        if (pos[i].x < board->w && pos[i].y < board->h)
            phput(board->wires, pos[i], wire);
    }

    ts_remove_wires_under_ics(board);

    ts_sandbox_start_simulation(board->sandbox);
    return TS_OK;
}

//
// components
//

ts_Result ts_board_add_component(ts_Board* board, const char* name, ts_Position pos, ts_Direction direction)
{
    PL_DEBUG("Tying to add component '%s' at %d,%d", name, pos.x, pos.y);

    ts_Result r = TS_OK;

    if (pos.dir != TS_CENTER)
        abort();

    ts_sandbox_end_simulation(board->sandbox);

    // find component
    ts_ComponentDef const* def = ts_component_db_def(&board->sandbox->component_db, name);
    if (def == NULL)
        PL_ERROR_RET(TS_COMPONENT_NOT_FOUND, "Component '%s' not found in database.", name);

    // is it inside the board?
    ts_Rect component_rect = ts_component_def_rect(def, pos, direction);
    ts_Rect board_rect = { (ts_Position) { 0, 0, TS_CENTER }, (ts_Position) { board->w - 1, board->h - 1, TS_CENTER } };
    if (!ts_rect_a_inside_b(component_rect, board_rect)) {
        PL_DEBUG("Component outside of the board");
        goto skip;
    }

    // is there another component here? (TODO)
    ts_HashPosComponentPtr other_components = ts_board_component_tiles(board);
    for (int x = component_rect.top_left.x; x <= component_rect.bottom_right.x; ++x) {
        for (int y = component_rect.top_left.y; y <= component_rect.bottom_right.y; ++y) {
            ts_Position p = { x, y, TS_CENTER };
            if (phgeti(other_components, p) >= 0) {
                phfree(other_components);
                PL_DEBUG("Trying to put component on top of another component");
                goto skip;
            }
        }
    }
    phfree(other_components);

    // init component
    ts_Component* component = calloc(1, sizeof(ts_Component));
    r = ts_component_init(component, def, direction);
    if (r != TS_OK) {
        PL_DEBUG("Error initializing component");
        goto skip;
    }

    // place component
    phput(board->components, pos, component);  // pointer owns the object
    ts_component_update_pos(component, board, pos);
    PL_DEBUG("Component placed.");

    // remove wires underneath
    if (component->def->type != TS_SINGLE_TILE)
        ts_board_remove_wires_for_ic(board, component_rect);

skip:
    ts_sandbox_start_simulation(board->sandbox);
    return r;
}

ts_Component* ts_board_component(ts_Board const* board, ts_Position pos)
{
    if (pos.dir != TS_CENTER)
        abort();

    ts_Component* ret = NULL;

    int i = phgeti(((ts_Board *) board)->components, pos);
    if (i >= 0) {
        ret = board->components[i].value;
    } else {
        ts_HashPosComponentPtr comps = ts_board_component_tiles(board);
        int j = phgeti(comps, pos);
        if (j >= 0)
            ret = comps[j].value;
        phfree(comps);
    }

    return ret;
}

//
// tile
//

ts_Result ts_board_rotate_tile(ts_Board const* board, ts_Position pos)
{
    if (pos.dir != TS_CENTER)
        abort();

    ts_sandbox_end_simulation(board->sandbox);

    ts_Component* component = ts_board_component(board, pos);
    if (component != NULL && component->def->can_rotate)
        component->direction = ts_direction_rotate_component(component->direction);
    PL_DEBUG("Component '%s' rotated to '%s'", component->def->key, ts_direction_serialize(component->direction));

    ts_sandbox_start_simulation(board->sandbox);
    return TS_OK;
}

ts_Result ts_board_clear_tile(ts_Board const* board, ts_Position pos)
{
    PL_DEBUG("Clearing component tile %d,%d", pos.x, pos.y);

    if (pos.dir != TS_CENTER)
        abort();

    ts_sandbox_end_simulation(board->sandbox);

    // clear component
    ts_Component* component = ts_board_component(board, pos);
    if (component) {
        ts_component_finalize(component);
        phdel(((ts_Board *) board)->components, component->position);
        free(component);
    }

    // clear wires
    ts_Position p;
    p = (ts_Position) { pos.x, pos.y, TS_N }; phdel(((ts_Board *) board)->wires, p);
    p = (ts_Position) { pos.x, pos.y, TS_E }; phdel(((ts_Board *) board)->wires, p);
    p = (ts_Position) { pos.x, pos.y, TS_S }; phdel(((ts_Board *) board)->wires, p);
    p = (ts_Position) { pos.x, pos.y, TS_W }; phdel(((ts_Board *) board)->wires, p);

    ts_sandbox_start_simulation(board->sandbox);
    return TS_OK;
}

ts_HashPosComponentPtr ts_board_component_tiles(ts_Board const* board)
{
    ts_HashPosComponentPtr list = NULL;

    for (int i = 0; i < phlen(board->components); ++i) {
        ts_Component* component = board->components[i].value;
        ts_Rect rect = ts_component_rect(component);
        for (int x = rect.top_left.x; x <= rect.bottom_right.x; ++x) {
            for (int y = rect.top_left.y; y <= rect.bottom_right.y; ++y) {
                ts_Position pos = { x, y, TS_CENTER };
                phput(list, pos, component);
            }
        }
    }

    return list;
}

//
// serialization
//

int ts_board_serialize(ts_Board const* board, int vspace, FILE* f)
{
    SR_INIT("{");
    SR_CONT("  w = %d,", board->w);
    SR_CONT("  h = %d,", board->h);
    SR_CONT("  wires = {");
    for (int i = 0; i < phlen(board->wires); ++i) {
        char key[30]; ts_pos_serialize(ts_pos_unhash(board->wires[i].key), key, sizeof key);
        SR_CONT_INLINE("    [%s] = ", key);
        SR_CALL_INLINE(ts_wire_serialize, &board->wires[i].value);
        SR_ADD_INLINE(",\n");
    }
    SR_CONT("  },");
    SR_CONT("  components = {");
    for (int i = 0; i < phlen(board->components); ++i) {
        char key[30]; ts_pos_serialize(ts_pos_unhash(board->components[i].key), key, sizeof key);
        SR_CONT_INLINE("    [%s] = ", key);
        SR_CALL(ts_component_serialize, board->components[i].value, 4);
        SR_ADD_INLINE(",\n");
    }
    SR_CONT("  },");
    SR_CONT("},");
    return TS_OK;
}

static ts_Result ts_board_unserialize_wires(ts_Board* board, lua_State* L, ts_Sandbox* sb)
{
    lua_getfield(L, -1, "wires");
    if (!lua_istable(L, -1))
        PL_ERROR_RET(TS_DESERIALIZATION_ERROR, "Expected a table 'wires'");
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        ts_Position pos;
        ts_Wire wire;
        ts_Result r;

        lua_pushvalue(L, -2);
        if ((r = ts_pos_unserialize(&pos, L)) != TS_OK)
            return r;
        lua_pop(L, 1);
        if ((r = ts_wire_unserialize(&wire, L, sb) != TS_OK))
            return r;
        phput(board->wires, pos, wire);
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    return TS_OK;
}

static ts_Result ts_board_unserialize_components(ts_Board* board, lua_State* L, ts_Sandbox* sb)
{
    lua_getfield(L, -1, "components");
    if (!lua_istable(L, -1))
        PL_ERROR_RET(TS_DESERIALIZATION_ERROR, "Expected a table 'components'");
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        ts_Position  pos;
        ts_Component* component = calloc(1, sizeof(ts_Component));
        ts_Result  r;

        lua_pushvalue(L, -2);
        if ((r = ts_pos_unserialize(&pos, L)) != TS_OK)
            return r;
        lua_pop(L, 1);
        if ((r = ts_component_unserialize(component, L, sb) != TS_OK))
            return r;
        phput(board->components, pos, component);
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    return TS_OK;
}

ts_Result ts_board_unserialize(ts_Board* board, lua_State* L, ts_Sandbox* sb)
{
    ts_Result r;

    lua_getfield(L, -1, "w"); int w = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_getfield(L, -1, "h"); int h = luaL_checkinteger(L, -1); lua_pop(L, 1);

    ts_board_init(board, sb, w, h);

    r = ts_board_unserialize_wires(board, L, sb);
    if (r == TS_OK)
        r = ts_board_unserialize_components(board, L, sb);

    ts_sandbox_start_simulation(board->sandbox);
    return r;
}