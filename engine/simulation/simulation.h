#ifndef SIMULATION_H
#define SIMULATION_H

#include <pthread.h>

#include "basic/position.h"
#include "compiler/compiler.h"

typedef struct ts_Sandbox ts_Sandbox;

typedef struct ts_Simulation {
    ts_Connection*    connections;
    size_t            steps;
    ts_Sandbox const* sandbox;
} ts_Simulation;

// initialization
ts_Result ts_simulation_init(ts_Simulation* sim, ts_Sandbox* sandbox);
ts_Result ts_simulation_finalize(ts_Simulation* sim);

// start/stop (will recompile and start a new simulation - used when the board is changed)
ts_Result ts_simulation_start(ts_Simulation* sim);
ts_Result ts_simulation_end(ts_Simulation* sim);

// information
size_t    ts_simulation_wires(ts_Simulation const* sim, ts_Position* positions, uint8_t* values, size_t sz);
size_t    ts_simulation_steps(ts_Simulation* sim);

// execution
ts_Result ts_simulation_single_step(ts_Simulation* sim);
ts_Result ts_simulation_run(ts_Simulation* sim, size_t run_for_us);

#endif //SIMULATION_H
