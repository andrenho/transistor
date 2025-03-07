#ifndef SANDBOX_H
#define SANDBOX_H

#include <stdio.h>
#include <stddef.h>
#include <lua.h>

#include "util/result.h"
#include "component/componentdb.h"
#include "simulation/simulation.h"

typedef struct ts_Board ts_Board;

typedef struct ts_Sandbox {
    ts_Board*      boards;
    ts_ComponentDB component_db;
    ts_Simulation  simulation;
    lua_State*     L;
} ts_Sandbox;

// initialization
ts_Result ts_sandbox_init(ts_Sandbox* sb);
ts_Result ts_sandbox_finalize(ts_Sandbox* sb);

// simulation
ts_Result ts_sandbox_end_simulation(ts_Sandbox* sb);
ts_Result ts_sandbox_start_simulation(ts_Sandbox* sb);

// serialization
ts_Result ts_sandbox_serialize(ts_Sandbox const* sb, int vspace, FILE* f);
ts_Result ts_sandbox_unserialize_from_string(ts_Sandbox* sb, const char* str);
ts_Result ts_sandbox_unserialize_from_file(ts_Sandbox* sb, FILE* f);

#endif //SANDBOX_H
