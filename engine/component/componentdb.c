#include "componentdb.h"

#include <assert.h>
#include <pl_log.h>
#include <string.h>

#include <stb_ds.h>

#include "component.h"
#include "sandbox/sandbox.h"

//
// category cache
//

static int subcategory_compare(const void* a, const void* b)
{
    ts_SubCatCache const *sa = a, *sb = b;
    if (sa->category == sb->category) {
        return strcmp(sa->subcategory, sb->subcategory);
    }
    return sa->category - sb->category;
}

static void update_subcategory_db(ts_ComponentDB* db)
{
    // clear subcategory cache
    arrfree(db->subcategories_cache);
    db->subcategories_cache = NULL;

    // recreate subcategory cache
    for (int i = 0; i < shlen(db->items); ++i) {
        ts_ComponentDef const* def = &db->items[i];
        if (def->category && def->subcategory) {
            ts_SubCatCache cache_item = {
                .category = def->category,
                .subcategory = def->subcategory,
            };
            arrpush(db->subcategories_cache, cache_item);
        }
    }

    // order cache
    qsort(db->subcategories_cache, arrlen(db->subcategories_cache), sizeof(ts_SubCatCache), subcategory_compare);
}


//
// initialization
//

ts_Result ts_component_db_init(ts_ComponentDB* db, ts_Sandbox* sb)
{
    memset(db, 0, sizeof(ts_ComponentDB));
    db->sandbox = sb;
    db->subcategories_cache = NULL;
    PL_DEBUG("Component database created.");
    return TS_OK;
}

ts_Result ts_component_db_finalize(ts_ComponentDB* db)
{
    arrfree(db->subcategories_cache);
    for (int i = 0; i < shlen(db->items); ++i)
        ts_component_def_finalize(&db->items[i]);
    shfree(db->items);
    PL_DEBUG("Component database finalized.");
    return TS_OK;
}

//
// db management
//

ts_Result ts_component_db_add_def_from_lua(ts_ComponentDB* db, const char* lua_code, int graphics_luaref, bool included)
{
    PL_DEBUG("Adding Lua component def to database");

    ts_ComponentDef def;
    ts_Result r = ts_component_def_init_from_lua(&def, lua_code, db->sandbox, graphics_luaref, included);
    if (r != TS_OK)
        return r;

    if (shgeti(db->items, def.key) >= 0)
        PL_ERROR_RET(TS_COMPONENT_ALREADY_IN_DB, "Component '%s' already exist in database.", def.key);

    shputs(db->items, def);
    PL_DEBUG("Component def'%s' added", def.key);

    update_subcategory_db(db);
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

ts_Result ts_component_db_clear_not_included(ts_ComponentDB* db)
{
    char** to_remove = NULL;
    for (int i = 0; i < shlen(db->items); ++i)
        if (!db->items[i].included)
            arrpush(to_remove, db->items[i].key);

    for (int i = 0; i < arrlen(to_remove); ++i) {
        int j = shgeti(db->items, to_remove[i]);
        ts_component_def_finalize(&db->items[j]);
        shdel(db->items, to_remove[i]);
    }
    arrfree(to_remove);

    update_subcategory_db(db);
    return TS_OK;
}

//
// queries
//

ts_ComponentDef const* ts_component_db_def(ts_ComponentDB const* db, const char* name)
{
    ts_ComponentDef const* def = shgetp_null(((ts_ComponentDB *) db)->items, name);
    if (def == NULL)
        PL_ERROR_RET(NULL, "Component '%s' not found in database.", name);
    return def;
}

ts_Result ts_component_db_init_component(ts_ComponentDB const* db, const char* name, ts_Component* component)
{
    ts_ComponentDef const* def = ts_component_db_def(db, name);
    if (def == NULL)
        PL_ERROR_RET(TS_COMPONENT_NOT_FOUND, "Component '%s' not found in database.", name);
    return ts_component_init(component, def, TS_N);
}

size_t ts_component_db_subcategories(ts_ComponentDB const* db, ts_ComponentCategory category, const char* subcategories[], int max_subcategories)
{
    size_t j = 0;
    for (int i = 0; i < arrlen(db->subcategories_cache); ++i) {
        ts_SubCatCache* cache = &db->subcategories_cache[i];
        if (cache->category == category) {
            subcategories[j++] = cache->subcategory;
            if (j == max_subcategories)
                break;
        }
    }
    return j;
}

static int r_strcmp(const void* a, const void* b)
{
    return strcmp(a, b);
}

size_t ts_component_db_subcategory_defs(ts_ComponentDB const* db, ts_ComponentCategory category, const char* subcategory, const char* defs[], int max_defs)
{
    size_t j = 0;
    for (int i = 0; i < shlen(db->items); ++i) {
        ts_ComponentDef const* def = &db->items[i];
        if (def->category == category && strcmp(def->subcategory, subcategory) == 0 && def->name) {
            defs[j++] = def->name;
            if (j == max_defs)
                break;
        }
    }
    qsort(defs, j, sizeof(const char*), r_strcmp);
    return j;
}

//
// serialization
//

int ts_component_db_serialize(ts_ComponentDB const* db, int vspace, FILE* f)
{
    for (int i = 0; i < shlen(db->items); ++i) {
        if (!db->items[i].included) {
            fprintf(f, "%*s", vspace, "");
            ts_component_def_serialize(&db->items[i], f);
            fprintf(f, ",\n");
        }
    }
    return TS_OK;
}

ts_Result ts_component_db_unserialize(ts_ComponentDB* db, lua_State* LL, ts_Sandbox* sb)
{
    ts_component_db_clear_not_included(db);
    size_t len = lua_objlen(LL, -1);
    for (size_t i = 0; i < len; ++i) {
        lua_rawgeti(LL, -1, i + 1);
        ts_component_db_add_def_from_lua(db, lua_tostring(LL, -1), -1, false);
        lua_pop(LL, 1);
    }
    update_subcategory_db(db);
    return TS_OK;
}
