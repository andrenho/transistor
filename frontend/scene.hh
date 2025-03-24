#ifndef FRONTENDSCENERENDERER_HH
#define FRONTENDSCENERENDERER_HH

#include <pastel2d.hh>
#include <transistor.hh>

class SceneMapper : public Transistor::SceneRenderer {
public:
    explicit SceneMapper(std::vector<ps::Scene>& scenes)
        : scenes(scenes) {}

    void render(lua_State* L) override;

private:
    std::vector<ps::Scene>& scenes;

    static ps::Context context(lua_State* L);
    static void instruction(lua_State* L, ps::Scene& scene);
};

#endif //FRONTENDSCENERENDERER_HH
