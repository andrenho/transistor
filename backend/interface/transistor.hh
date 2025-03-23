#ifndef TRANSISTOR_H_
#define TRANSISTOR_H_

#include <lua.hpp>

class Transistor {
public:
    Transistor();
    ~Transistor();

    bool run_tests();

private:
    lua_State* L;

    void setup();
};

#endif
