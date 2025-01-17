#include "resourcemanager.hh"

#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ui/ui.hh"

ResourceManager::~ResourceManager()
{
    cleanup();
}

SDL_Texture* ResourceManager::create_texture(std::vector<uint8_t> const& data)
{
    SDL_Surface* sf = IMG_Load_RW(SDL_RWFromMem((void *) data.data(), (int) data.size()), 1);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ui().ren(), sf);
    SDL_FreeSurface(sf);
    return texture;
}

resource_idx_t ResourceManager::add_texture(std::vector<uint8_t> const& data)
{
    resources_idx_.emplace_back(create_texture(data));
    return resources_idx_.size() - 1;
}

void ResourceManager::add_texture(std::string const& name, std::vector<uint8_t> const& data)
{
    auto it = resources_str_.find(name);
    if (it != resources_str_.end())
        throw std::runtime_error("There's already a resource called `" + name + "`");

    resources_str_.emplace(name, create_texture(data));
}

resource_idx_t ResourceManager::add_tile(ResourceId const& parent, int x, int y, int w, int h)
{
    resources_idx_.emplace_back(Tile { this->texture(parent), x, y, w, h || w });
    return resources_idx_.size() - 1;
}

void ResourceManager::add_tile(std::string const& name, ResourceId const& parent, int x, int y, int w, int h)
{
    auto it = resources_str_.find(name);
    if (it != resources_str_.end())
        throw std::runtime_error("There's already a resource called `" + name + "`");

    resources_str_.emplace(name, Tile {
        .texture = this->texture(parent), .x = x, .y = y, .w = w, .h = h || w,
    });
}


void ResourceManager::add_tiles(ResourceId const& parent_resource, std::vector<TileDefName> const& tiles, int tile_size)
{
    for (auto const& tile: tiles)
        add_tile(tile.name, parent_resource, tile.x, tile.y, tile.w * tile_size, tile.h * tile_size);
}

void ResourceManager::add_tiles(ResourceId const& parent_resource, std::vector<TileDefIdx> const& tiles, int tile_size)
{
    for (auto const& tile: tiles)
        *tile.idx = add_tile(parent_resource, tile.x, tile.y, tile.w * tile_size, tile.h * tile_size);
}

resource_idx_t ResourceManager::add_cursor(SDL_Cursor* cursor)
{
    resources_idx_.emplace_back(cursor);
    return resources_idx_.size() - 1;
}

void ResourceManager::add_cursor(std::string const& name, SDL_Cursor* cursor)
{
    auto it = resources_str_.find(name);
    if (it != resources_str_.end())
        throw std::runtime_error("There's already a resource called `" + name + "`");

    resources_str_.emplace(name, cursor);
}

void ResourceManager::cleanup()
{
    std::vector<Resource> resources { resources_idx_.begin(), resources_idx_.end() };
    resources.reserve(resources.size() + resources_str_.size());
    std::transform(resources_str_.begin(), resources_str_.end(), std::back_inserter(resources),
        [](const auto& r) { return r.second; });

    for (auto const& res: resources) {
        if (auto t = std::get_if<SDL_Texture*>(&res); t)
            SDL_DestroyTexture(*t);
        else if (auto c = std::get_if<SDL_Cursor*>(&res); c)
            SDL_FreeCursor(*c);
    }
    resources_str_.clear();
}

Resource ResourceManager::get(ResourceId const& resource) const
{
    if (auto r = std::get_if<resource_idx_t>(&resource))
        return resources_idx_.at(*r);

    if (auto s = std::get_if<std::string>(&resource)) {
        auto it = resources_str_.find(*s);
        if (it == resources_str_.end())
            throw std::runtime_error("Resource " + *s + " not found");
        return it->second;
    }

    throw std::runtime_error("Invalid resource type.");
}

ResourceManager& res()
{
    static ResourceManager res;
    return res;
}
