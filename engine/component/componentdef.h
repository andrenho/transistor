#ifndef COMPONENTDEF_H
#define COMPONENTDEF_H

#include <stdbool.h>
#include <stdint.h>

#include "board/board.h"
#include "board/wire.h"

typedef struct ts_Component ts_Component;

typedef enum { TS_SINGLE_TILE, TS_IC_DIP, TS_IC_QUAD } ts_ComponentType;

typedef enum { TS_INPUT, TS_OUTPUT } ts_PinDirection;

typedef enum { TS_CAT_LOGIC_GATES, TS_CAT_DIGITAL, TS_CAT_MEMORY, TS_CAT_CPU } ts_ComponentCategory;

typedef struct ts_PinDef {
    char*           name;
    ts_PinDirection direction;
    ts_WireWidth    wire_width;
} ts_PinDef;

typedef void (*SimulateFn)(ts_Component* component);

typedef struct ts_ComponentDef {
    char*                key;               // component index in the database
    ts_ComponentType     type;              // single tile, ic_dup (PDIP) or ic_quad
    bool                 can_rotate;
    uint8_t              ic_width;          // in PDIP, distance between the two rows

    char*                name;              // component name, as it'll show up for the user
    ts_ComponentCategory category;          // define where it'll show up in the tool bar
    char*                subcategory;

    uint8_t              n_pins;
    ts_PinDef*           pins;

    size_t               data_size;         // number of bytes used as data

    SimulateFn           c_simulate;        // call this instead of 'simulate' on Lua, if present

    ts_Sandbox*         sandbox;
    int                 luaref;            // store global reference in Lua

    bool                included;          // included components are provided by the application and not serialized/deserialized

} ts_ComponentDef;

// initialization
ts_Result ts_component_def_init_from_lua(ts_ComponentDef* def, const char* lua_code, ts_Sandbox* sb, int graphics_luaref, bool included);
ts_Result ts_component_def_finalize(ts_ComponentDef* def);

// positioning
ts_Rect ts_component_def_rect(ts_ComponentDef const* def, ts_Position component_pos, ts_Direction dir);
size_t  ts_component_def_pin_positions(ts_ComponentDef const* def, ts_Position component_pos, ts_Direction dir,
                                       ts_PinPos* pin_pos, size_t max_pin_pos);

// serialization
ts_Result ts_component_def_serialize(ts_ComponentDef const* def, FILE* f);
ts_Result ts_component_def_unserialize(ts_ComponentDef* def, lua_State* L, ts_Sandbox* sb);

#endif //COMPONENTDEF_H
