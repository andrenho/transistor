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
    auto it = resources_.find(name);
    if (it != resources_.end())
        throw std::runtime_error("There's already a resource called `" + name + "`");

    ui();
    SDL_Surface* sf = IMG_Load_RW(SDL_RWFromMem((void *) data.data(), (int) data.size()), 1);
    SDL_Texture* texture_ = SDL_CreateTextureFromSurface(ui().ren(), sf);
    SDL_FreeSurface(sf);
    resources_.emplace(name, texture_);
}

void ResourceManager::add_tile(std::string const& name, std::string const& texture, int x, int y, int w, int h)
{
    auto it = resources_.find(name);
    if (it != resources_.end())
        throw std::runtime_error("There's already a resource called `" + name + "`");

    resources_.emplace(name, Tile {
        .texture = this->texture(texture),
        .x = x,
        .y = y,
        .w = w,
        .h = h,
    });
}

void ResourceManager::add_tiles(std::string const& parent_resource, std::vector<TileDef> const& tiles, int tile_size)
{
    for (auto const& tile: tiles)
        add_tile(tile.name, parent_resource, tile.x, tile.y, tile.w * tile_size, tile.h * tile_size);
}

void ResourceManager::add_cursor(std::string const& name, SDL_Cursor* cursor)
{
    auto it = resources_.find(name);
    if (it != resources_.end())
        throw std::runtime_error("There's already a resource called `" + name + "`");

    resources_.emplace(name, cursor);
}

Resource ResourceManager::get(std::string const& name) const
{
    auto it = resources_.find(name);
    if (it == resources_.end())
        throw std::runtime_error("Resource " + name + " not found");
    return it->second;
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
