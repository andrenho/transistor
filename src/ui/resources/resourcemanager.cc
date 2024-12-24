#include "resourcemanager.hh"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Resource ResourceManager::from_image(std::vector<uint8_t> const& data)
{
    SDL_Surface* sf = IMG_Load_RW(SDL_RWFromMem((void *) data.data(), (int) data.size()), 1);
    SDL_Texture* texture_ = SDL_CreateTextureFromSurface(ren_, sf);
    SDL_FreeSurface(sf);
    return resources_.emplace_back(texture_);
}

Resource ResourceManager::from_image(Resource const& res, int x, int y, int w, int h)
{
    return resources_.emplace_back(Resource::SubTexture { res, x, y, w, h });
}

Resource ResourceManager::from_tileset(Resource const& res, int tile_sz, int x, int y, int w, int h)
{
    return from_image(res, x * tile_sz, y * tile_sz, w * tile_sz, h * tile_sz);
}

std::vector<Resource> ResourceManager::from_atlas(Resource const& res, std::vector<AtlasCoordinate> const& coordinates, int tile_size)
{
    std::vector<Resource> resources;
    resources.reserve(coordinates.size());
    for (auto const& c: coordinates)
        resources.push_back(from_tileset(res, tile_size, c.x, c.y, c.w, c.h));
    return resources;
}

void ResourceManager::cleanup()
{
    for (auto const& res: resources_) {
        if (auto t = std::get_if<SDL_Texture*>(&res.data); t)
            SDL_DestroyTexture(*t);
    }
    resources_.clear();
}

ResourceManager::~ResourceManager()
{
    cleanup();
}
