#ifndef LUAOBJ_DIALOG_HH_
#define LUAOBJ_DIALOG_HH_

#include <vector>
#include <lua.hpp>
#include "mappers/out/event.hh"

namespace luaobj {

struct Dialog {
    enum Type { Information, Question, Warning, Error };

    struct Button {
        std::string          text {};
        std::optional<Event> event {};

        static Button from_lua(lua_State* L, int index) {
            return {
                .LUAW_FIELD(text),
                .LUAW_FIELD(event),
            };
        }
    };

    std::string              title;
    std::vector<std::string> text;
    Type                     type;
    std::vector<Button>      buttons;
    size_t                   default_button;
    mutable bool             show = false;

    static Dialog from_lua(lua_State* L, int index) {
        Dialog dialog = {
            .LUAW_FIELD(title),
            .text = {},
            .type = Information,
            .LUAW_FIELD(buttons),
            .LUAW_FIELD(default_button),
        };

        luaw_getfield(L, -1, "text");
        if (luaw_is<std::string>(L, -1))
            dialog.text = { luaw_to_string(L, -1) };
        else
            dialog.text = luaw_to<std::vector<std::string>>(L, -1);
        lua_pop(L, 1);

        auto type = luaw_getfield<std::string>(L, -1, "type");
        if (type == "information")
            dialog.type = Information;
        else if (type == "question")
            dialog.type = Question;
        else if (type == "warning")
            dialog.type = Warning;
        else if (type == "error")
            dialog.type = Error;

        return dialog;
    }
};

}

#endif
