#ifndef RENDER_HH
#define RENDER_HH

#include <optional>
#include <string>
#include <vector>

#include "lua.hh"

using Callback = std::unique_ptr<LuaRef>;

struct Dialog {
    enum class Type { None, Information, Question, Warning, Error };
    std::string              title;
    std::vector<std::string> text;
    Type                     type;
    std::vector<std::string> buttons;
    size_t                   default_button;
    mutable bool             show;
    Callback const*          callback;
};

struct MenuItem {
    std::string              text;
    std::optional<size_t>    confirmation_dialog;
    Callback                 callback_ref;
    std::vector<MenuItem>    items {};
};

struct EngineCompilation {
    bool                       success = true;
    std::string                compilation_messages;
    std::optional<std::string> test_errors;
};

struct Render {
    EngineCompilation     engine_compilation;
    int                   steps_per_second = 0;
    std::vector<Dialog>   dialogs;
    std::vector<MenuItem> menus;

    void load_from_lua(lua_State* L);

private:
    size_t add_dialog(lua_State* L, Callback const* callback);
    std::vector<MenuItem> load_menus(lua_State* L);
};

#endif //RENDER_HH
