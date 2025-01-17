#include "resourcemanager.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ui/ui.hh"

ResourceManager::~ResourceManager()
{
    cleanup();
}

void ResourceManager::add_texture(std::string const& name, std::vector<uint8_t> const& data)
{
    ui();
    SDL_Surface* sf = IMG_Load_RW(SDL_RWFromMem((void *) data.data(), (int) data.size()), 1);
    SDL_Texture* texture_ = SDL_CreateTextureFromSurface(ui().ren(), sf);
    SDL_FreeSurface(sf);
    resources_.emplace(name, texture_);
}

void ResourceManager::add_tile(std::string const& name, std::string const& texture, int x, int y, int w, int h)
{
    resources_.emplace(name, Tile {
        .texture = this->texture(texture),
        .x = x,
        .y = y,
        .w = w,
        .h = h,
    });
}

void ResourceManager::add_tiles(std::string const& name, std::vector<TileDef> const& tiles)
{
    for (auto const& tile: tiles)
        add_tile(name, tile.name, tile.x, tile.y, tile.w, tile.h);
}

void ResourceManager::add_cursor(std::string const& name, SDL_Cursor* cursor)
{
    resources_.emplace(name, cursor);
}

void ResourceManager::cleanup()
{
    for (auto const& res: resources_) {
        if (auto t = std::get_if<SDL_Texture*>(&res.second); t)
            SDL_DestroyTexture(*t);
        else if (auto c = std::get_if<SDL_Cursor*>(&res.second); c)
            SDL_FreeCursor(*c);
    }
    resources_.clear();
}

ResourceManager& res()
{
    static ResourceManager res;
    return res;
}
