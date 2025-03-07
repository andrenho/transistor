#ifndef DIRECTION_H
#define DIRECTION_H
#include <lua.h>

#include "util/result.h"

typedef struct ts_Sandbox ts_Sandbox;

typedef enum ts_Direction {
    TS_CENTER, TS_N, TS_S, TS_W, TS_E,
} ts_Direction;

#define TS_DIRS_N 5

ts_Direction ts_direction_rotate_component(ts_Direction dir);

const char*  ts_direction_serialize(ts_Direction dir);
ts_Result    ts_direction_unserialize(ts_Direction* dir, lua_State* L);

#endif //DIRECTION_H
