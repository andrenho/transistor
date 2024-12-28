#ifndef RENDERCONTEXT_HH
#define RENDERCONTEXT_HH

#include "ui/scene.hh"
#include "ui/layers/board/circuit_atlas.hh"

struct ComponentRenderContext {
    Scene::ImageContext*         context;
    Scene&                       scene;
    std::vector<Resource> const& icons;

    void add_to_scene(Resource resource, int x, int y, Pen const& pen={}) const
    {
        scene.add(context, resource, x, y, pen);
    }

    void add_to_scene(CSprite sprite, int x, int y, Pen const& pen={}) const
    {
        scene.add(context, icons.at((size_t) sprite), x, y, pen);
    }
};

#endif //RENDERCONTEXT_HH
