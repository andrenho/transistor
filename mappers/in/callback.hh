#ifndef CALLBACK_HH
#define CALLBACK_HH

namespace luaobj {

struct Callback {
    std::string command;

    static Callback from_lua(lua_State* L, int index) {
        return {
            .command = luaw_getfield<std::string>(L, index, "command"),
        };
    }
};

}

#endif //CALLBACK_HH
