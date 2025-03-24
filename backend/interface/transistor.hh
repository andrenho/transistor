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

    class SceneRenderer {
    public:
        virtual ~SceneRenderer() = default;
        virtual void render(lua_State* L) = 0;
    };
    [[nodiscard]] Render render(SceneRenderer& scene_renderer) const;

private:
    Lua lua_;
    EngineCompilation engine_compilation_;

    void setup();
    std::pair<bool, std::string> check_engine();
    void setup_autoreload();
};

#endif
