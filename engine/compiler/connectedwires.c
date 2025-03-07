#include "connectedwires.h"

#include <stb_ds.h>

static ts_Position* ts_tile_neighbours(ts_Position p, bool has_single_tile_component_pin)
{
    ts_Position* positions = NULL;
    ts_Position pp;

    if (has_single_tile_component_pin) {

        switch (p.dir) {
            case TS_W: pp = (ts_Position) { p.x - 1, p.y, TS_E }; arrpush(positions, pp); break;
            case TS_E: pp = (ts_Position) { p.x + 1, p.y, TS_W }; arrpush(positions, pp); break;
            case TS_N: pp = (ts_Position) { p.x, p.y - 1, TS_S }; arrpush(positions, pp); break;
            case TS_S: pp = (ts_Position) { p.x, p.y + 1, TS_N }; arrpush(positions, pp); break;
            case TS_CENTER: abort();
        }

    } else {

        switch (p.dir) {
            case TS_W:
                pp = (ts_Position) { p.x - 1, p.y, TS_E }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_E }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_N }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_S }; arrpush(positions, pp);
                break;
            case TS_E:
                pp = (ts_Position) { p.x + 1, p.y, TS_W }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_W }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_N }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_S }; arrpush(positions, pp);
                break;
            case TS_N:
                pp = (ts_Position) { p.x, p.y - 1, TS_S }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_S }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_W }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_E }; arrpush(positions, pp);
                break;
            case TS_S:
                pp = (ts_Position) { p.x, p.y + 1, TS_N }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_N }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_W }; arrpush(positions, pp);
                pp = (ts_Position) { p.x, p.y, TS_E }; arrpush(positions, pp);
                break;
            case TS_CENTER:
                abort();
        }

    }

    return positions;
}

static ts_Position* ts_compiler_find_connected_group(ts_Position start, ts_PosSet* wires, ts_Position* single_tile_component_pins)
{
    ts_Position* result = NULL;
    ts_PosSet* to_visit = NULL;  // hashset

    psetput(to_visit, start);

    while (psetlen(to_visit) > 0) {
        ts_Position visiting = psetfirst(to_visit);

        if (psetcontains(wires, visiting)) {

            // add to result list, and remove from visited
            arrpush(result, visiting);
            psetdel(wires, visiting);

            // find if we have a single tile component pin here
            bool has_single_tile_component_pin = arrcontains(single_tile_component_pins, (ts_Position) { visiting.x, visiting.y, TS_CENTER });

            // add neighbours
            ts_Position* neighbours = ts_tile_neighbours(visiting, has_single_tile_component_pin);
            for (int i = 0; i < arrlen(neighbours); ++i)
                psetput(to_visit, neighbours[i]);
            arrfree(neighbours);
        }

        psetdel(to_visit, visiting);
    }

    psetfree(to_visit);
    return result;
}

PositionArray* ts_compiler_find_connected_wires(ts_PosSet* wires, ts_Position* single_tile_component_pins)
{
    if (psetlen(wires) == 0)
        return NULL;

    ts_Position** groups = NULL;
    while (psetlen(wires) > 0)
        arrpush(groups, ts_compiler_find_connected_group(psetfirst(wires), wires, single_tile_component_pins));

    return groups;
}

