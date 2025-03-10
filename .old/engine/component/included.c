#include "included.h"

#include "transistor-sandbox.h"

#define NATIVE_COMPONENTS 1

// embedded Lua files
#include "components/basic/button.lua.h"
#include "components/basic/led.lua.h"
#include "components/basic/npn.lua.h"
#include "components/basic/pnp.lua.h"
#include "components/basic/vcc.lua.h"
#include "components/gates/or_2i.lua.h"

static void button_sim(ts_Component* button)
{
    button->pins[0] = button->pins[1] = button->pins[2] = button->pins[3] = button->data[0];
}

static void led_sim(ts_Component* led)
{
    led->data[0] = led->pins[0] | led->pins[1] | led->pins[2] | led->pins[3];
}

static void vcc_sim(ts_Component* vcc)
{
    vcc->pins[0] = vcc->pins[1] = vcc->pins[2] = vcc->pins[3] = 1;
}

static void npn_sim(ts_Component* npn)
{
    static const size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
    npn->pins[OUT] = npn->pins[IN] & (npn->pins[SWITCH_1] | npn->pins[SWITCH_2]);
}

static void pnp_sim(ts_Component* pnp)
{
    static const size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
    pnp->pins[OUT] = pnp->pins[IN] & !(pnp->pins[SWITCH_1] | pnp->pins[SWITCH_2]);
}

static void or_2i_sim(ts_Component* c)
{
    static const size_t IN0 = 0, IN1 = 0, Q_ = 2, Q = 3;
    c->pins[Q] = c->pins[IN0] | c->pins[IN1];
    c->pins[Q_] = !c->pins[Q];
}

static void load_component(ts_Transistor* t, const char* lua_code)
{
    ts_component_db_add_from_lua(t, lua_code, true);
}

void ts_included_components_init(ts_Transistor* t)
{
    load_component(t, components_basic_button_lua_str);
    load_component(t, components_basic_led_lua_str);
    load_component(t, components_basic_npn_lua_str);
    load_component(t, components_basic_pnp_lua_str);
    load_component(t, components_basic_vcc_lua_str);
    load_component(t, components_gates_or_2i_lua_str);

#if NATIVE_COMPONENTS
    ts_component_db_native_simulation(t, "__button", button_sim);
    ts_component_db_native_simulation(t, "__led", led_sim);
    ts_component_db_native_simulation(t, "__npn", npn_sim);
    ts_component_db_native_simulation(t, "__pnp", pnp_sim);
    ts_component_db_native_simulation(t, "__vcc", vcc_sim);
    ts_component_db_native_simulation(t, "__or_2i", or_2i_sim);
#endif
}
