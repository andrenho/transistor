#ifndef RENDERCONTEXT_HH
#define RENDERCONTEXT_HH

#include "ui/scene.hh"
#include "ui/layers/board/circuit_atlas.hh"

struct ComponentRenderContext {
    Scene::ImageContext const*   context;
    Scene&                       scene;
    std::vector<Resource> const& icons;
    int                          x, y;

    void add_to_scene(Resource resource, int x_, int y_, Pen const& pen={}) const
    {
        scene.add(context, resource, x + x_, y + y_, pen);
    }

    void add_to_scene(CSprite sprite, int x_, int y_, Pen const& pen={}) const
    {
        scene.add(context, icons.at((size_t) sprite), x + x_, y + y_, pen);
    }
};

#endif //RENDERCONTEXT_HH
