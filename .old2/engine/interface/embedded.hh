#ifndef EMBEDDED_HH
#define EMBEDDED_HH

struct Bytecode {
    const char* name;
    uint8_t const* data;
    size_t sz;
};

#include "engine/util/strict.lua.h"
#include "engine/util/serialize.lua.h"
#include "engine/util/util.lua.h"
#include "engine/board.lua.h"
#include "engine/compiler.lua.h"
#include "engine/component.lua.h"
#include "engine/componentdb.lua.h"
#include "engine/componentdef.lua.h"
#include "engine/cursor.lua.h"
#include "engine/geometry.lua.h"
#include "engine/sandbox.lua.h"
#include "engine/wire.lua.h"

static std::vector<Bytecode> engine_lua = {
#define LOAD(name) { #name, engine_engine_##name##_lua, engine_engine_##name##_lua_sz }
    LOAD(util_strict),
    LOAD(util_serialize),
    LOAD(util_util),
    LOAD(board),
    LOAD(compiler),
    LOAD(component),
    LOAD(componentdb),
    LOAD(componentdef),
    LOAD(cursor),
    LOAD(geometry),
    LOAD(sandbox),
    LOAD(wire),
#undef LOAD
};

#include "engine/components/basic/button.lua.h"
#include "engine/components/basic/led.lua.h"
#include "engine/components/basic/npn.lua.h"
#include "engine/components/basic/pnp.lua.h"
#include "engine/components/basic/vcc.lua.h"
#include "engine/components/gates/or_2i.lua.h"

static std::vector<Bytecode> components_lua = {
#define COMP(name) { #name, engine_components_##name##_lua, engine_components_##name##_lua_sz }
    COMP(basic_button),
    COMP(basic_led),
    COMP(basic_npn),
    COMP(basic_pnp),
    COMP(basic_vcc),
    COMP(gates_or_2i),
#undef COMP
};

#endif //EMBEDDED_HH
