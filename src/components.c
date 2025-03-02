#include "components.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include "button.lua.h"
#include "graphics.h"

static void load_component(ts_Transistor* t, const char* lua_code)
{
    ts_Result r = ts_transistor_component_db_add_from_lua(t, lua_code);
    if (r != TS_OK) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading default component", ts_last_error(&t->sandbox, NULL),
            ps_graphics_window());
        abort();
    }

    // TODO - run init
}

void components_init(ts_Transistor* t)
{
    load_component(t, resources_components_button_lua);
}
