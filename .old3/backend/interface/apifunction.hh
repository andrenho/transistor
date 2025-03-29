#ifndef APIFUNCTION_HH
#define APIFUNCTION_HH

#include <string>
#include <lua.hpp>

struct ApiFunction {
    std::string command;

    static ApiFunction parse(lua_State* L);
};

#endif //APIFUNCTION_HH
