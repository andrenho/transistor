#ifndef COMPONENTS_HH
#define COMPONENTS_HH

#include <lua.hpp>

#include "component/componentdb.h"

constexpr const char* button = R"(
return {
    key = "__button",
    type = "single_tile",

    can_rotate = true,  -- not real
    data_size = 1,

    pins = {
        { name = "O1", direction = "output" },
        { name = "O2", direction = "output" },
        { name = "O3", direction = "output" },
        { name = "O4", direction = "output" },
    },

    on_click = function(button)
        button.data[1] = bit.bnot(button.data[1])
    end,

    simulate = function(button)
        for i=1,#button.pin do button.pin[i] = button.data[1] end
    end,
})";

constexpr const char* led = R"(
return {
    key = "__led",
    type = "single_tile",

    can_rotate = false,
    data_size = 1,

    pins = {
        { name = "I1", direction = "input" },
        { name = "I2", direction = "input" },
        { name = "I3", direction = "input" },
        { name = "I4", direction = "input" },
    },

    simulate = function(led)
        led.data[1] = bit.bor(led.pin[1], led.pin[2], led.pin[3], led.pin[4])
    end,
})";

constexpr const char* or_2i = R"(
return {
    key = "__or_2i",
    type = "ic_dip",

    can_rotate = true,

    pins = {
        { name = "IN0", direction = "input" },
        { name = "IN1", direction = "input" },
        { name = "~Q",  direction = "output" },
        { name = "Q",   direction = "output" },
    },

    simulate = function(c)
        IN0, IN1, Q_, Q = 1, 2, 3, 4
        c.pin[Q] = bit.bor(c.pin[IN0], c.pin[IN1])
        c.pin[Q_] = bit.bnot(c.pin[Q])
    end,
})";

inline void ts_add_lua_components(ts_Sandbox* sb)
{
    ts_component_db_add_def_from_lua(&sb->component_db, button, -1);
    ts_component_db_add_def_from_lua(&sb->component_db, led, -1);
    ts_component_db_add_def_from_lua(&sb->component_db, or_2i, -1);
}

#endif //COMPONENTS_HH
