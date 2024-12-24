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
