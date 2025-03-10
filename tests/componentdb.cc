#include "doctest.h"

#include <string>

extern "C" {
#include <transistor-sandbox.h>
}

static const std::string custom_ic_1 = R"(return
    {
        key = "my1",
        type = "single_tile",
        pins = {},

        name = "aaa",
        category = "digital",
        subcategory = "bbb",
    }
)";

static const std::string custom_ic_2 = R"(return
    {
        key = "my2",
        type = "single_tile",
        pins = {},

        name = "ggg",
        category = "digital",
        subcategory = "ddd",
    }
)";

static const std::string custom_ic_3 = R"(return
    {
        key = "my3",
        type = "single_tile",
        pins = {},

        name = "eee",
        category = "memory",
        subcategory = "fff",
    }
)";

static const std::string custom_ic_4 = R"(return
    {
        key = "my4",
        type = "single_tile",
        pins = {},

        name = "ccc",
        category = "digital",
        subcategory = "ddd",
    }
)";


TEST_SUITE("Component DB")
{
    TEST_CASE("Query subcategories")
    {
        ts_Transistor T;
        ts_init(&T, { false, TS_CPU_NORMAL }, nullptr);

        ts_component_db_add_from_lua(&T, custom_ic_2.c_str(), false);  // out-of-order
        ts_component_db_add_from_lua(&T, custom_ic_1.c_str(), false);
        ts_component_db_add_from_lua(&T, custom_ic_3.c_str(), false);
        ts_component_db_add_from_lua(&T, custom_ic_4.c_str(), false);

        const char* subcategories[3];
        size_t sz = ts_subcategories(&T, TS_CAT_DIGITAL, subcategories, 3);
        CHECK(sz == 2);
        CHECK(std::string(subcategories[0]) == "bbb");
        CHECK(std::string(subcategories[1]) == "ddd");

        const char* names[3];
        sz = ts_subcategory_defs(&T, TS_CAT_DIGITAL, "ddd", names, 3);
        CHECK(sz == 2);
        CHECK(std::string(names[0]) == "ccc");
        CHECK(std::string(names[1]) == "ggg");

        ts_finalize(&T);
    }
}