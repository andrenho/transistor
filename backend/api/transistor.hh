#ifndef TRANSISTOR_H_
#define TRANSISTOR_H_

#include <lua.hpp>

class Transistor {
public:
    Transistor();
    ~Transistor();

private:
    lua_State* L;
};

#endif
