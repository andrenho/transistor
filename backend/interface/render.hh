#ifndef RENDER_HH
#define RENDER_HH

#include <lua.hpp>
#include <optional>
#include <string>
#include <vector>

#include "lua.hh"

struct MenuItem {
    std::string                text;
    std::optional<std::string> ask_confirmation {};
    // int                        callback_ref;
    std::vector<MenuItem>      items {};
};

struct EngineCompilation {
    bool                       success = true;
    std::string                compilation_messages;
    std::optional<std::string> test_errors;
};

struct Render {
    EngineCompilation     engine_compilation;
    int                   steps_per_second = 0;
    std::vector<MenuItem> menus;

    void load_from_lua(lua_State* L);

private:
    void load_menu(lua_State* L, MenuItem& menu);
};

#endif //RENDER_HH
