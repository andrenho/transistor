#ifndef LUAENGINE_HH
#define LUAENGINE_HH

class LuaEngine {
public:
    LuaEngine();
    ~LuaEngine();

    void hello() const;

private:
    struct lua_State* L;
};

#endif //LUAENGINE_HH
