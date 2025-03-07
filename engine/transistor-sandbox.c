#include "transistor-sandbox.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

#include <lua.h>
#include <lauxlib.h>
#include <pl_log.h>

#include "stb_ds.h"
#include "sandbox/sandbox.h"

const char* ts_version()
{
    return PROJECT_VERSION;
}

//
// execution thread
//

static void* thread_run(void* ptr_t)
{
    ts_Transistor* t = ptr_t;

    PL_DEBUG("Starting simulation thread");
    while (t->thread_running) {

        // pause thread
        pthread_mutex_lock(&t->lock);
        while (t->thread_paused)
            pthread_cond_wait(&t->cond, &t->lock);

        // execute
        ts_simulation_single_step(&t->sandbox.simulation);

        // unpause
        pthread_mutex_unlock(&t->lock);

        // give the CPU a break
        if (!t->config.heavy)
            sched_yield();
    }

    return NULL;
}

static void thread_init(ts_Transistor* t)
{
    if (t->config.multithreaded) {
        t->thread_running = true;
        pthread_mutex_init(&t->lock, NULL);
        pthread_cond_init(&t->cond, NULL);
        pthread_create(&t->thread, NULL, thread_run, t);
    }
}

//
// initialization
//


ts_Result ts_init(ts_Transistor* t, ts_TransistorConfig config)
{
    PL_INFO("Initializing transistor library in %s mode", config.multithreaded ? "multithreaded" : "single-threaded");

    memset(t, 0, sizeof(ts_Transistor));
    ts_sandbox_init(&t->sandbox);
    t->config = config;
    thread_init(t);
    return TS_OK;
}

ts_Result ts_unserialize(ts_Transistor* t, ts_TransistorConfig config, const char* str)
{
    PL_DEBUG("Staring deserialization");

    memset(t, 0, sizeof(ts_Transistor));
    ts_Result r = ts_sandbox_unserialize_from_string(&t->sandbox, str);
    if (r != 0)
        return r;
    t->config = config;
    thread_init(t);

    PL_DEBUG("Deserialization complete");
    return TS_OK;
}

ts_Result ts_unserialize_from_file(ts_Transistor* t, ts_TransistorConfig config, FILE* f)
{
    char* buffer;
    ssize_t bytes_read = getdelim(&buffer, NULL, '\0', f);
    if (bytes_read < 0)
        PL_ERROR_RET(TS_SYSTEM_ERROR, "Error reading file: %s", strerror(errno));
    PL_DEBUG("File read with %zi bytes", bytes_read);
    ts_Result r = ts_unserialize(t, config, buffer);
    free(buffer);
    return r;
}

ts_Result ts_finalize(ts_Transistor* t)
{
    if (t->config.multithreaded) {
        t->thread_running = false;
        pthread_join(t->thread, NULL);
    }
    ts_sandbox_finalize(&t->sandbox);
    PL_INFO("Transistor finalized");
    return TS_OK;
}

//
// serialization
//

ts_Result ts_serialize_to_file(ts_Transistor const* t, FILE* f)
{
    return ts_sandbox_serialize(&t->sandbox, 0, f);
}

//
// locking
//

ts_Result ts_lock(ts_Transistor* t)
{
    if (t->config.multithreaded && !t->thread_paused) {
        PL_TRACE("Thread lock.");
        pthread_mutex_lock(&t->lock);
        t->thread_paused = true;
    }
    return TS_OK;
}

ts_Result ts_unlock(ts_Transistor* t)
{
    if (t->config.multithreaded && t->thread_paused) {
        PL_TRACE("Thread unlock.");
        t->thread_paused = false;
        pthread_cond_signal(&t->cond);
        pthread_mutex_unlock(&t->lock);
    }
    return TS_OK;
}

//
// boards
//

ts_BoardIdx ts_add_board(ts_Transistor* t, int w, int h)
{
    abort();  // TODO - not implemented yet
}

//
// component db
//

ts_Result ts_component_db_add_from_lua(ts_Transistor* t, const char* lua_code, int graphics_luaref)
{
    ts_lock(t);
    ts_Result r = ts_component_db_add_def_from_lua(&t->sandbox.component_db, lua_code, graphics_luaref);
    ts_unlock(t);
    return r;
}

ts_Result ts_component_db_native_simulation(ts_Transistor* t, const char* name, SimulateFn sim_fn)
{
    ts_lock(t);
    ts_component_db_update_simulation(&t->sandbox.component_db, name, sim_fn);
    ts_unlock(t);
    return TS_OK;
}

//
// execution
//

ts_Result ts_run(ts_Transistor* t, size_t run_for_us)
{
    if (!t->config.multithreaded)
        return ts_simulation_run(&t->sandbox.simulation, run_for_us);
    return TS_OK;
}

//
// cursor
//

ts_Result ts_on_cursor_set_position(ts_Transistor* t, ts_BoardIdx board_idx, ts_Position pos)
{
    ts_Result r;
    ts_lock(t);
    if (pos.x < 0 || pos.y < 0 || pos.x >= t->sandbox.boards[board_idx].w || pos.y >= t->sandbox.boards[board_idx].h)
        r = ts_cursor_set_pointer_out_of_bounds(&t->sandbox.boards[board_idx].cursor);
    else
        r = ts_cursor_set_pointer(&t->sandbox.boards[board_idx].cursor, pos);
    ts_unlock(t);
    return r;
}

ts_Result ts_on_cursor_click(ts_Transistor* t, ts_BoardIdx board_idx, ts_CursorButton button)
{
    ts_lock(t);
    ts_Result r = ts_cursor_click(&t->sandbox.boards[board_idx].cursor, button);
    ts_unlock(t);
    return r;
}

ts_Result ts_on_cursor_release(ts_Transistor* t, uint8_t button)
{
    ts_Result r = TS_OK;
    ts_lock(t);
    for (int i = 0; i < arrlen(t->sandbox.boards); ++i) {
        r = ts_cursor_release(&t->sandbox.boards[i].cursor, button);
        if (r != TS_OK)
            break;
    }
    ts_unlock(t);
    return r;
}

ts_Result ts_on_cursor_key_press(ts_Transistor* t, ts_BoardIdx board_idx, char key, uint8_t keymod)
{
    ts_lock(t);
    ts_Result r = ts_cursor_key_press(&t->sandbox.boards[board_idx].cursor, key, keymod);
    ts_unlock(t);
    return r;
}

ts_Result ts_on_key_release(ts_Transistor* t, char key)
{
    ts_Result r = TS_OK;
    ts_lock(t);
    for (int i = 0; i < arrlen(t->sandbox.boards); ++i) {
        r = ts_cursor_key_release(&t->sandbox.boards[i].cursor, key);
        if (r != TS_OK)
            break;
    }
    ts_unlock(t);
    return r;
}

ts_Result ts_on_select_component_def(ts_Transistor* t, const char* name)
{
    ts_Result r = TS_OK;
    ts_lock(t);
    for (int i = 0; i < arrlen(t->sandbox.boards); ++i) {
        r = ts_cursor_select_component_def(&t->sandbox.boards[i].cursor, name);
        if (r != TS_OK)
            break;
    }
    ts_unlock(t);
    return r;
}

//
// snapshots
//

static void add_component_def(ts_ComponentDef const* def, ts_Component const* component, ts_Position pos, ts_Direction dir, bool cursor, ts_ComponentSnapshot* dest)
{
    *dest = (ts_ComponentSnapshot) {
        .key = strdup(def->key),
        .pos = pos,
        .direction = dir,
        .type = def->type,
        .ic_width = def->ic_width,
        .n_pins = def->n_pins,
        .pins = calloc(def->n_pins, sizeof(ts_PinSnapshot)),
        .cursor = cursor,
        .def_luaref = def->luaref,
        .luaref = component ? component->luaref : -1,
    };

    // pins
    for (int k = 0; k < def->n_pins; ++k) {
        ts_PinDef* pin = &def->pins[k];
        dest->pins[k] = (ts_PinSnapshot) {
            .name = strdup(pin->name),
            .type = pin->direction,
        };
    }
}

ts_Result ts_take_snapshot(ts_Transistor const* t, ts_TransistorSnapshot* snap)
{
    const size_t MAX_WIRE_CURSOR = 2000;

    PL_TRACE("Creating snapshot...");

    snap->boards = calloc(arrlen(t->sandbox.boards), sizeof(ts_BoardSnapshot));
    snap->n_boards = arrlen(t->sandbox.boards);
    for (int i = 0; i < arrlen(t->sandbox.boards); ++i) {

        ts_Board* board = &t->sandbox.boards[i];

        // get wire cursor positions
        size_t n_wire_cursor = 0;
        ts_Position wire_cursor[MAX_WIRE_CURSOR];
        if (board->cursor.wire.drawing)
            n_wire_cursor = ts_pos_a_to_b(board->cursor.wire.starting_pos, board->cursor.pos, board->cursor.wire.orientation, wire_cursor, MAX_WIRE_CURSOR);

        // board
        size_t n_components = phlen(board->components) + (board->cursor.selected_def ? 1 : 0);
        size_t n_wires = phlen(board->wires) + n_wire_cursor;
        snap->boards[i] = (ts_BoardSnapshot) {
            .w = board->w,
            .h = board->h,
            .n_components = n_components,
            .components = calloc(n_components, sizeof(ts_ComponentSnapshot)),
            .n_wires = n_wires,
            .wires = calloc(n_wires, sizeof(ts_WireSnapshot)),
        };

        // components
        int j;
        for (j = 0; j < phlen(board->components); ++j) {
            ts_Component* component = board->components[j].value;
            add_component_def(component->def, component, ts_pos_unhash(board->components[j].key), component->direction, false, &snap->boards[i].components[j]);
        }
        if (board->cursor.selected_def)
            add_component_def(board->cursor.selected_def, NULL, board->cursor.pos, board->cursor.selected_direction, true, &snap->boards[i].components[j]);

        // wires
        const int max = phlen(board->wires);
        ts_Position positions[max];
        uint8_t values[max];
        size_t sz = ts_simulation_wires(&t->sandbox.simulation, positions, values, max);
        int k = 0;
        for (size_t m = 0; m < sz; ++m) {
            ts_Wire* wire = ts_board_wire(board, positions[m]);
            if (wire) {
                snap->boards[i].wires[k++] = (ts_WireSnapshot) {
                    .pos = positions[m],
                    .layer = wire->layer,
                    .width = wire->width,
                    .value = values[m],
                    .cursor = false,
                };
            }
        }

        // cursor wires
        if (board->cursor.wire.drawing) {
            for (size_t m = 0; m < n_wire_cursor; ++m) {
                snap->boards[i].wires[k++] = (ts_WireSnapshot) {
                    .pos = wire_cursor[m],
                    .layer = board->cursor.selected_wire.layer,
                    .width = board->cursor.selected_wire.width,
                    .value = 0,
                    .cursor = true,
                };
            }
        }

        snap->boards[i].n_wires = k;
    }

    if (pl_level() == PL_TRACE_LEVEL) {
        PL_TRACE("Snapshot created with %zu boards.", snap->n_boards);
        for (size_t i = 0; i < snap->n_boards; ++i)
            PL_TRACE("Board %zu has %zu wires and %zu components", i, snap->boards[i].n_wires, snap->boards[i].n_components);
    }

    return TS_OK;
}

ts_Result ts_snapshot_finalize(ts_TransistorSnapshot* snap)
{
    for (size_t i = 0; i < snap->n_boards; ++i) {
        for (size_t j = 0; j < snap->boards[i].n_components; ++j) {
            for (size_t k = 0; k < snap->boards[i].components[j].n_pins; ++k)
                free(snap->boards[i].components[j].pins[k].name);
            free(snap->boards[i].components[j].pins);
            free(snap->boards[i].components[j].key);
        }
        free(snap->boards[i].components);
        free(snap->boards[i].wires);
    }
    free(snap->boards);
    PL_TRACE("Snapshot finalized.");
    return TS_OK;
}

ts_Result ts_component_onclick(ts_Transistor* t, ts_ComponentSnapshot const* component)
{
    lua_State* L = t->sandbox.L;

    lua_rawgeti(L, LUA_REGISTRYINDEX, component->def_luaref);
    lua_getfield(L, -1, "onclick");
    if (!lua_isnil(L, -1)) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, component->luaref);
        int r = lua_pcall(L, 1, 0, 0);
        if (r != LUA_OK) {
            const char* error = lua_tostring(L, -1);
            lua_pop(L, 2);
            PL_ERROR_RET(TS_LUA_FUNCTION_ERROR, "lua function 'onclick' error: %s", error);
        }
    } else {
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return TS_OK;
}

static int direction_angle(ts_Direction dir)
{
    switch (dir) {
        case TS_CENTER: return 0;
        case TS_N: return 0;
        case TS_E: return 90;
        case TS_S: return 180;
        case TS_W: return 270;
    }
    return 0;
}

ts_Result ts_component_render(ts_Transistor* t, ts_ComponentSnapshot const* component, int graphics_luaref, int x, int y)
{
    ts_lock(t);

    PL_TRACE("Calling LUA function 'render' for component");

    lua_State* L = t->sandbox.L;

    // LUA call
    lua_rawgeti(L, LUA_REGISTRYINDEX, component->def_luaref);
    lua_getfield(L, -1, "render");

    // parameters (component, G, x, y)
    lua_rawgeti(L, LUA_REGISTRYINDEX, component->luaref);
    lua_rawgeti(L, LUA_REGISTRYINDEX, graphics_luaref);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);

    // create context
    lua_newtable(L);
    lua_pushnumber(L, direction_angle(component->direction));
    lua_setfield(L, -2, "rotation");
    if (component->cursor) {
        lua_pushnumber(L, .5f);
        lua_setfield(L, -2, "opacity");
    }

    int r = lua_pcall(L, 5, 0, 0);
    if (r != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        lua_pop(L, 2);
        PL_ERROR_RET(TS_LUA_FUNCTION_ERROR, "lua function 'render' error: %s", error);
    }
    lua_pop(L, 1);
    ts_unlock(t);

    return TS_OK;
}

int ts_steps_per_second(ts_Transistor* t)
{
    struct timeval now;
    gettimeofday(&now, NULL);

    if (timercmp(&now, &t->next_step_calculation, >=)) {
        ts_lock(t);
        int total_steps = ts_simulation_steps(&t->sandbox.simulation);
        ts_unlock(t);
        t->steps_per_second = total_steps - t->last_step_count;
        t->last_step_count = total_steps;

        struct timeval one_second = { .tv_sec = 1, .tv_usec = 0 };
        timeradd(&now, &one_second, &t->next_step_calculation);
    }

    return t->steps_per_second;
}

void ts_borrow_lua(ts_Transistor* t, void(*f)(lua_State* L, void* data), void* data)
{
    ts_lock(t);
    f(t->sandbox.L, data);
    ts_unlock(t);
}