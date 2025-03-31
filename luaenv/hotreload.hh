#ifndef HOTRELOAD_HH
#define HOTRELOAD_HH

#include "lua.hh"

class HotReload {
public:
    HotReload(Lua& lua);
    ~HotReload();

    bool restart();

private:
    bool restart_ = false;
};

#endif //HOTRELOAD_HH
