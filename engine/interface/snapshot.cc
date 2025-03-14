#include "snapshot.hh"

#include <cassert>
#include <cstring>

static Direction lua_todirection(lua_State* L, int i)
{
    const char* dir = lua_tostring(L, i);
    switch (dir[0]) {
        case 'N': return Direction::N;
        case 'S': return Direction::S;
        case 'W': return Direction::W;
        case 'E': return Direction::E;
        case 'C': return Direction::Center;
        default: abort();
    }
}

Snapshot parse_snapshot(lua_State* L)
{
    Snapshot snap;

    assert(lua_gettop(L) == 1);
    assert(lua_istable(L, -1));

    // boards
    lua_getfield(L, -1, "boards");
    assert(lua_istable(L, -1));
    lua_pushnil(L);
    while (lua_next(L, -2)) {

        Snapshot::Board board;

        lua_getfield(L, -1, "id"); board.id = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "w");  board.w = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "h");  board.h = lua_tointeger(L, -1); lua_pop(L, 1);

        // components
        lua_getfield(L, -1, "components");
        assert(lua_istable(L, -1));
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            Snapshot::Component component;
            lua_rawgeti(L, -1, 1); component.x = lua_tointeger(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 2); component.y = lua_tointeger(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 3); component.direction = lua_todirection(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 4); component.key = lua_tostring(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 5); component.id = lua_tointeger(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 6); component.data_size = lua_tointeger(L, -1); lua_pop(L, 1);
            board.components.emplace_back(std::move(component));
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        // wires
        lua_getfield(L, -1, "wires");
        assert(lua_istable(L, -1));
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            Snapshot::Wire wire;
            lua_rawgeti(L, -1, 1); wire.x = lua_tointeger(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 2); wire.y = lua_tointeger(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 3); wire.direction = lua_todirection(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 4);
                const char* wr = lua_tostring(L, -1);
                wire.layer = wr[0] == 'T' ? Snapshot::Layer::Top : Snapshot::Layer::Bottom;
                wire.width = wr[1] == '1' ? Snapshot::Width::W1 : Snapshot::Width::W8;
            lua_pop(L, 1);
            lua_rawgeti(L, -1, 5); wire.id = lua_tointeger(L, -1); lua_pop(L, 1);
            board.wires.emplace_back(std::move(wire));
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        snap.boards.emplace_back(std::move(board));
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    assert(lua_gettop(L) == 1);

    return snap;
}

void hydrate_snapshot_with_values(Snapshot& snapshot, CompilationResult const& result)
{
    for (auto& board: snapshot.boards) {
        for (auto& wire: board.wires)
            wire.value = result.connections.at(result.connection_by_wire_id.at(wire.id)).value;
        for (auto& component: board.components) {
            component.data.resize(component.data_size);
            memcpy(component.data.data(), result.components.at(result.component_by_id.at(component.id)).data, component.data_size);
        }
    }
}
