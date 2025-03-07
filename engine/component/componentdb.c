#include "componentdb.h"

#include <assert.h>
#include <pl_log.h>
#include <string.h>

#include <stb_ds.h>

#include "component.h"
#include "sandbox/sandbox.h"

ts_Result ts_component_db_init(ts_ComponentDB* db, ts_Sandbox* sb)
{
    memset(db, 0, sizeof(ts_ComponentDB));
    db->sandbox = sb;
    PL_DEBUG("Component database created.");
    return TS_OK;
}

ts_Result ts_component_db_finalize(ts_ComponentDB* db)
{
    for (int i = 0; i < shlen(db->items); ++i)
        ts_component_def_finalize(&db->items[i]);
    shfree(db->items);
    PL_DEBUG("Component database finalized.");
    return TS_OK;
}

ts_Result ts_component_db_add_def_from_lua(ts_ComponentDB* db, const char* lua_code, int graphics_luaref)
{
    PL_DEBUG("Adding Lua component def to database");

    ts_ComponentDef def;
    ts_Result r = ts_component_def_init_from_lua(&def, lua_code, db->sandbox, graphics_luaref);
    if (r != TS_OK)
        return r;
    shputs(db->items, def);
    PL_DEBUG("Component def'%s' added", def.key);
    return TS_OK;
}

ts_Result ts_component_db_update_simulation(ts_ComponentDB* db, const char* name, SimulateFn sim_fn)
{
    PL_DEBUG("Updating component '%s' to use native simulation.", name);

    ts_ComponentDef* def = (ts_ComponentDef *) ts_component_db_def(db, name);
    if (def == NULL)
        PL_ERROR_RET(TS_COMPONENT_NOT_FOUND, "Component definition '%s' not found in database.", name);
    def->c_simulate = sim_fn;
    return TS_OK;
}

ts_ComponentDef const* ts_component_db_def(ts_ComponentDB const* db, const char* name)
{
    return shgetp_null(((ts_ComponentDB *) db)->items, name);
}

ts_Result ts_component_db_init_component(ts_ComponentDB const* db, const char* name, ts_Component* component)
{
    ts_ComponentDef const* def = ts_component_db_def(db, name);
    if (def == NULL)
        PL_ERROR_RET(TS_COMPONENT_NOT_FOUND, "Component '%s' not found in database.", name);
    return ts_component_init(component, def, TS_N);
}

int ts_component_db_serialize(ts_ComponentDB const* db, int vspace, FILE* f)
{
    for (int i = 0; i < shlen(db->items); ++i) {
        fprintf(f, "%*s", vspace, "");
        ts_component_def_serialize(&db->items[i], f);
        fprintf(f, ",\n");
    }
    return TS_OK;
}

ts_Result ts_component_db_unserialize(ts_ComponentDB* db, lua_State* LL, ts_Sandbox* sb)
{
    ts_component_db_init(db, sb);
    size_t len = lua_objlen(LL, -1);
    for (size_t i = 0; i < len; ++i) {
        ts_ComponentDef def;
        lua_rawgeti(LL, -1, i + 1);
        ts_component_db_add_def_from_lua(db, lua_tostring(LL, -1), -1);
        lua_pop(LL, 1);
    }
    return TS_OK;
}
