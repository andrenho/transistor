#ifndef COMPONENTDB_HH
#define COMPONENTDB_HH

#include "util/result.h"
#include "componentdef.h"
#include <lua.h>

typedef struct ts_Sandbox ts_Sandbox;

typedef struct {
    ts_ComponentCategory category;
    const char*          subcategory;
} ts_SubCatCache;

typedef struct ts_ComponentDB {
    ts_Sandbox*      sandbox;
    ts_ComponentDef* items;
    ts_SubCatCache*  subcategories_cache;
} ts_ComponentDB;

// initialization
ts_Result ts_component_db_init(ts_ComponentDB* db, ts_Sandbox* sb);
ts_Result ts_component_db_finalize(ts_ComponentDB* db);

// db management
ts_Result              ts_component_db_add_def_from_lua(ts_ComponentDB* db, const char* lua_code, int graphics_luaref, bool included);
ts_Result              ts_component_db_update_simulation(ts_ComponentDB* db, const char* name, SimulateFn sim_fn);
ts_Result              ts_component_db_clear_not_included(ts_ComponentDB* db);

// queries
ts_ComponentDef const* ts_component_db_def(ts_ComponentDB const* db, const char* name);
size_t                 ts_component_db_subcategories(ts_ComponentDB const* db, ts_ComponentCategory category, const char* subcategories[], int max_subcategories);
size_t                 ts_component_db_subcategory_defs(ts_ComponentDB const* db, ts_ComponentCategory category, const char* subcategory, ts_ComponentDef const* defs[], int max_defs);

// serialization
int       ts_component_db_serialize(ts_ComponentDB const* db, int vspace, FILE* f);
ts_Result ts_component_db_unserialize(ts_ComponentDB* db, lua_State* LL, ts_Sandbox* sb);

#endif //COMPONENTDB_HH
