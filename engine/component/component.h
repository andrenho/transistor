#ifndef COMPONENT_H
#define COMPONENT_H

#include <lua.h>
#include <stdint.h>
#include <stdio.h>

#include "basic/position.h"
#include "util/result.h"
#include "pinpos.h"
#include "basic/rect.h"

typedef struct ts_ComponentDef ts_ComponentDef;
typedef struct ts_Sandbox ts_Sandbox;
typedef struct ts_Board ts_Board;

typedef struct ts_Component {
    ts_ComponentDef const* def;
    ts_Direction           direction;
    uint8_t*               data;
    uint8_t*               pins;
    ts_Board const*        board;
    ts_Position            position;
    int                    luaref;
} ts_Component;

// initialization
ts_Result ts_component_init(ts_Component* component, ts_ComponentDef const* def, ts_Direction direction);
ts_Result ts_component_finalize(ts_Component* component);

// updates
ts_Result ts_component_update_pos(ts_Component* component, ts_Board const* board, ts_Position pos);

// functions
ts_Result ts_component_on_click(ts_Component* component);

// positioning
ts_Rect ts_component_rect(ts_Component const* component);
size_t  ts_component_pin_positions(ts_Component const* component, ts_PinPos* pin_pos, size_t max_pin_pos);

// serialization
int       ts_component_serialize(ts_Component const* component, int vspace, FILE* f);
ts_Result ts_component_unserialize(ts_Component* component, lua_State* L, ts_Sandbox* sb);

#endif //COMPONENT_H
