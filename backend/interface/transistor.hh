#ifndef TRANSISTOR_H_
#define TRANSISTOR_H_

#include "lua.hh"
#include "render.hh"

class Transistor {
public:
    Transistor();
    ~Transistor();

    void                         init();
    std::pair<bool, std::string> run_tests();

    void                         update();

    void execure_ref_with_sandbox(LuaRef const& ref);

    class SceneRenderer {
    public:
        virtual ~SceneRenderer() = default;
        virtual void render(lua_State* L) = 0;
    };
    [[nodiscard]] Render render(SceneRenderer& scene_renderer) const;

private:
    Lua lua_;
    mutable EngineCompilation engine_compilation_;
    bool reset_lua_ = false;

    void setup();
    std::pair<bool, std::string> check_engine();
    void setup_autoreload();

    void lua_execute(lua_State* L, int n_pars, int n_results=0) const;
};

#endif
