#include "direction.h"

#include <string.h>
#include <stdlib.h>

#include <lauxlib.h>
#include <pl_log.h>

#include "sandbox/sandbox.h"

ts_Direction ts_direction_rotate_component(ts_Direction dir)
{
    switch (dir) {
        case TS_N: return TS_E;
        case TS_E: return TS_S;
        case TS_S: return TS_W;
        case TS_W: return TS_N;
        case TS_CENTER: return TS_CENTER;
    }
    abort();
}

const char* ts_direction_serialize(ts_Direction dir)
{
    switch (dir) {
        case TS_CENTER: return "CENTER";
        case TS_N:      return "N";
        case TS_S:      return "S";
        case TS_W:      return "W";
        case TS_E:      return "E";
    }
    return "invalid";
}

ts_Result ts_direction_unserialize(ts_Direction* dir, lua_State* L)
{
    const char* s = luaL_checkstring(L, -1);
    if (strcmp(s, "CENTER") == 0)
        *dir = TS_CENTER;
    else if (strcmp(s, "N") == 0)
        *dir = TS_N;
    else if (strcmp(s, "S") == 0)
        *dir = TS_S;
    else if (strcmp(s, "W") == 0)
        *dir = TS_W;
    else if (strcmp(s, "E") == 0)
        *dir = TS_E;
    else
        PL_ERROR_RET(TS_DESERIALIZATION_ERROR, "Direction invalid: '%s'", s);
    return TS_OK;
}
