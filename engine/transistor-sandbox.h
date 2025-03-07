#ifndef TRANSISTOR_SANDBOX_H
#define TRANSISTOR_SANDBOX_H

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#include "sandbox/sandbox.h"

typedef struct ts_Sandbox ts_Sandbox;

typedef struct ts_TransistorConfig {
    bool multithreaded;
    bool heavy;
} ts_TransistorConfig;

typedef struct ts_Transistor {
    ts_Sandbox          sandbox;
    ts_TransistorConfig config;

    pthread_t           thread;
    pthread_mutex_t     lock;
    pthread_cond_t      cond;
    bool                thread_running;
    bool                thread_paused;

    int                 steps_per_second;
    int                 last_step_count;
    struct timeval      next_step_calculation;
} ts_Transistor;

typedef unsigned int ts_BoardIdx;

// version
const char* ts_version();

// initialization
ts_Result ts_init(ts_Transistor* t, ts_TransistorConfig config);
ts_Result ts_unserialize(ts_Transistor* t, ts_TransistorConfig config, const char* str);
ts_Result ts_unserialize_from_file(ts_Transistor* t, ts_TransistorConfig config, FILE* f);
ts_Result ts_finalize(ts_Transistor* t);

// serialization
ts_Result ts_serialize_to_file(ts_Transistor const* t, FILE* f);

// locks (generally not needed)
ts_Result ts_lock(ts_Transistor* t);
ts_Result ts_unlock(ts_Transistor* t);

// boards
ts_BoardIdx ts_add_board(ts_Transistor* t, int w, int h);

// component db
ts_Result ts_component_db_add_from_lua(ts_Transistor* t, const char* lua_code, int graphics_luaref);
ts_Result ts_component_db_native_simulation(ts_Transistor* t, const char* name, SimulateFn sim_fn);

// execution
ts_Result ts_run(ts_Transistor* t, size_t run_for_us);

// cursor
ts_Result ts_on_cursor_set_position(ts_Transistor* t, ts_BoardIdx board_idx, ts_Position pos);
ts_Result ts_on_cursor_click(ts_Transistor* t, ts_BoardIdx board_idx, ts_CursorButton button);
ts_Result ts_on_cursor_release(ts_Transistor* t, uint8_t button);
ts_Result ts_on_cursor_key_press(ts_Transistor* t, ts_BoardIdx board_idx, char key, uint8_t keymod);
ts_Result ts_on_key_release(ts_Transistor* t, char key);
ts_Result ts_on_select_component_def(ts_Transistor* t, const char* name);

// other information
int       ts_steps_per_second(ts_Transistor* t);

//
// take snapshot
//

typedef struct ts_PinSnapshot {
    char*      name;
    ts_PinDirection type;
} ts_PinSnapshot;

typedef struct ts_ComponentSnapshot {
    char*            key;
    ts_Position      pos;
    ts_Direction     direction;
    ts_ComponentType type;
    uint8_t          ic_width;
    uint8_t          n_pins;
    ts_PinSnapshot*  pins;
    bool             cursor;
    int              def_luaref;
    int              luaref;
} ts_ComponentSnapshot;

typedef struct ts_WireSnapshot {
    ts_Position  pos;
    ts_WireWidth width;
    ts_WireLayer layer;
    uint8_t      value;
    bool         cursor;
} ts_WireSnapshot;

typedef struct ts_BoardSnapshot {
    int                   w;
    int                   h;
    ts_ComponentSnapshot* components;
    size_t                n_components;
    ts_WireSnapshot*      wires;
    size_t                n_wires;
} ts_BoardSnapshot;

typedef struct ts_TransistorSnapshot {
    ts_BoardSnapshot* boards;
    size_t            n_boards;
} ts_TransistorSnapshot;

ts_Result ts_take_snapshot(ts_Transistor const* t, ts_TransistorSnapshot* snap);
ts_Result ts_snapshot_finalize(ts_TransistorSnapshot* snap);

// execute Lua functions

ts_Result ts_component_onclick(ts_Transistor* t, ts_ComponentSnapshot const* component);
ts_Result ts_component_render(ts_Transistor* t, ts_ComponentSnapshot const* component, int graphics_luaref, int x, int y); // TODO - add context

// lock multithreaded execution, and borrow the Lua state
void ts_borrow_lua(ts_Transistor* t, void(*f)(lua_State* L, void* data), void* data);

#endif //TRANSISTOR_SANDBOX_H
