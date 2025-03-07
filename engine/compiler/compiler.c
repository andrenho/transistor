#include "compiler.h"

#include <pl_log.h>
#include <stb_ds.h>

#include "sandbox/sandbox.h"
#include "component/component.h"
#include "component/pinpos.h"
#include "basic/pos_ds.h"
#include "connectedwires.h"

ts_Pin* ts_compiler_find_all_pins(ts_Sandbox const* sb)
{
    ts_Pin* pins = NULL;
    for (int i = 0; i < arrlen(sb->boards); ++i) {
        for (int j = 0; j < phlen(sb->boards[i].components); ++j) {
            ts_Component* component = sb->boards[i].components[j].value;
            ts_PinPos pin_pos[component->def->n_pins];
            ts_component_pin_positions(component, pin_pos, component->def->n_pins);

            for (int k = 0; k < component->def->n_pins; ++k) {
                ts_Pin pin = {
                    &sb->boards[i],
                    component,
                    pin_pos[k].pos,
                    pin_pos[k].pin_no
                };
                arrpush(pins, pin);
            }
        }
    }
    return pins;
}

ts_Connection* ts_compiler_compile(ts_Sandbox const* sb)
{
    PL_DEBUG("Running compiler...");

    ts_Connection* connections = NULL;

    // reset all component pins
    for (int i = 0; i < arrlen(sb->boards); ++i) {
        ts_Board* board = &sb->boards[i];
        for (int j = 0; j < hmlen(board->components); ++j) {
            ts_Component* component = board->components[j].value;
            for (int k = 0; k < component->def->n_pins; ++k)
                component->pins[k] = 0;
        }
    }

    // create set of pin positions
    ts_Pin* pins = ts_compiler_find_all_pins(sb);

    // find single-tile component pins
    ts_Position* single_tile_component_pins = NULL;
    for (int i = 0; i < arrlen(pins); ++i) {
        if (pins[i].component->def->type == TS_SINGLE_TILE) {
            ts_Position p = { pins[i].pos.x, pins[i].pos.y, TS_CENTER };
            arrpush(single_tile_component_pins, p);
        }
    }

    // create set of wires
    ts_PosSet* wire_set = NULL;
    for (int i = 0; i < arrlen(sb->boards); ++i)
        for (int j = 0; j < phlen(sb->boards[i].wires); ++j)
            psetput(wire_set, ts_pos_unhash(sb->boards[i].wires[j].key));  // TODO - separate by width and layer

    // find groups, and create connections
    PositionArray* groups = ts_compiler_find_connected_wires(wire_set, single_tile_component_pins);
    for (int i = 0; i < arrlen(groups); ++i) {

        ts_Connection connection = {};

        // add wires to connection, and add to the list of central pins
        ts_PosSet* central_pins = NULL;
        for (int j = 0; j < arrlen(groups[i]); ++j) {

            // add to central_pins
            ts_Position p = { groups[i][j].x, groups[i][j].y, TS_CENTER }; psetput(central_pins, p);

            // add wire to connection
            arrpush(connection.wires, groups[i][j]);

            // add pin (single-tile components) to connection
            for (int k = 0; k < arrlen(pins); ++k)
                if (ts_pos_equals(groups[i][j], pins[k].pos))
                    arrpush(connection.pins, pins[k]);
        }

        // add pins (IC)
        for (int j = 0; j < psetlen(central_pins); ++j)
            for (int k = 0; k < arrlen(pins); ++k)
                if (ts_pos_equals(psetget(central_pins, j), pins[k].pos))
                    arrpush(connection.pins, pins[k]);

        psetfree(central_pins);

        arrpush(connections, connection);
    }

    // free stuff
    for (int i = 0; i < arrlen(groups); ++i)
        arrfree(groups[i]);
    arrfree(groups);
    psetfree(wire_set);
    arrfree(single_tile_component_pins);
    arrfree(pins);

    // debugging
    if (pl_level() == PL_DEBUG_LEVEL) {
        PL_DEBUG("Compilation complete: %ld connections found", arrlen(connections));
        for (int i = 0; i < arrlen(connections); ++i) {
            PL_DEBUG("- Connection #%d", i);
            ts_Connection* c = &connections[i];
            for (int j = 0; j < arrlen(c->pins); ++j)
                PL_DEBUG("    Pin %d of component '%s' at %d,%d",
                    c->pins[j].pin_no, c->pins[j].component->def->key, c->pins[j].component->position.x, c->pins[j].component->position.y);
        }
    }


    return connections;
}

ts_Result ts_connection_finalize(ts_Connection* connection)
{
    arrfree(connection->pins);
    arrfree(connection->wires);
    return TS_OK;
}
