#ifndef TRANSISTOR_H_
#define TRANSISTOR_H_

#include "lua.hh"

class Transistor {
public:
    Transistor();
    ~Transistor();

    bool run_tests();

private:
    Lua lua_;

    static void setup(lua_State* L);
};

#endif
