#include <lua.hpp>
#include <pl_log.h>

#include "doctest.h"

#include <string>

extern "C" {
#include "sandbox/sandbox.h"
#include "component/componentdef.h"
}

constexpr const char* components = R"(return
    {
        key = "__button",
        type = "single_tile",

        can_rotate = false,
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

        -- rendering

        init = function(button, renderer)
            puts('Button initialized.')
        end,

        render = function(button, renderer, x, y) end,
    }
)";


TEST_SUITE("Load IC from Lua")
{

    TEST_CASE("Custom single-tile data")
    {
        ts_Sandbox sb;
        ts_sandbox_init(&sb);

        ts_ComponentDef def;
        ts_Result r = ts_component_def_init_from_lua(&def, components, &sb, -1);
        if (r != TS_OK)
            fprintf(stderr, "error: %s\n", pl_last_error());

        CHECK(r == TS_OK);
        CHECK(std::string(def.key) == "__button");
        CHECK(def.type == TS_SINGLE_TILE);
        CHECK(def.can_rotate == false);
        CHECK(def.ic_width == 1);
        CHECK(def.data_size == 1);
        CHECK(def.n_pins == 4);
        CHECK(std::string(def.pins[1].name) == "O2");

        ts_component_def_finalize(&def);
        ts_sandbox_finalize(&sb);
    }

    TEST_CASE("Custom IC functions (Lua)")
    {
        ts_Sandbox sb;
        ts_sandbox_init(&sb);

        ts_ComponentDef def;
        ts_component_def_init_from_lua(&def, components, &sb, -1);
        ts_Component component;
        ts_component_init(&component, &def, TS_N);

        CHECK(component.data[0] == 0);
        if (ts_component_on_click(&component) != TS_OK)
            fprintf(stderr, "error: %s\n", pl_last_error());
        CHECK(component.data[0] != 0);

        ts_component_finalize(&component);
        ts_component_def_finalize(&def);
        ts_sandbox_finalize(&sb);
    }

    TEST_CASE("Multiple ICs")
    {
    }
}
