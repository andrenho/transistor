#ifndef DIALOG_HH_
#define DIALOG_HH_

#include <lua.hpp>

namespace luaobj {

struct Dialog {
    static Dialog from_lua(lua_State* L, int index) {
        return {};
    }
};

}

#endif //DIALOG_HH_
