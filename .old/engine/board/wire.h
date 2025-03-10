#ifndef WIRE_H
#define WIRE_H

#include <lua.h>
#include <stdio.h>

#include "util/result.h"

struct ts_Sandbox;

typedef enum { TS_WIRE_1 = '1', TS_WIRE_8 = '8' } ts_WireWidth;
typedef enum { TS_LAYER_TOP = 'T', TS_LAYER_BOTTOM = 'B' } ts_WireLayer;

typedef struct ts_Wire {
    ts_WireWidth width;
    ts_WireLayer layer;
} ts_Wire;

int         ts_wire_serialize(ts_Wire const* wire, int vspace, FILE* f);
ts_Result ts_wire_unserialize(ts_Wire* wire, lua_State* L, struct ts_Sandbox* sb);

#endif //WIRE_H
