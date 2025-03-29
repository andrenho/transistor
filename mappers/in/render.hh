#ifndef RENDER_HH
#define RENDER_HH

#include <luaw.hh>

class Render {
public:
    static Render from_lua(lua_State* L, int index);
};

#endif //RENDER_HH
