#ifndef CONNECTEDWIRES_H
#define CONNECTEDWIRES_H

#include "basic/pos_ds.h"

typedef ts_Position* PositionArray;

PositionArray* ts_compiler_find_connected_wires(ts_PosSet* wires, ts_Position* single_tile_component_pins);

#endif //CONNECTEDWIRES_H
