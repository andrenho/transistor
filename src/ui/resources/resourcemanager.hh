#ifndef RESOURCEMANAGER_HH
#define RESOURCEMANAGER_HH

#include "resource.hh"

#include <cstdint>
#include <vector>

struct SDL_Renderer;

struct AtlasCoordinate {
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t w = 1;
    uint16_t h = 1;
};

class ResourceManager {
public:
    ~ResourceManager();

    Resource from_image(std::vector<uint8_t> const& data);
    Resource from_image(Resource const& res, int x, int y, int w, int h);
    Resource from_tileset(Resource const& res, int tile_sz, int x, int y, int w=1, int h=1);
    std::vector<Resource> from_atlas(Resource const& res, std::vector<AtlasCoordinate> const& coordinates, int tile_size = 1);

    void cleanup();

    void set_renderer(SDL_Renderer* ren) { ren_ = ren; }

private:
    SDL_Renderer* ren_ = nullptr;
    std::vector<Resource> resources_;
};

#endif //RESOURCEMANAGER_HH
