#ifndef RESOURCEMANAGER_HH
#define RESOURCEMANAGER_HH

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "battery/embed.hpp"

struct SDL_Texture;
struct SDL_Cursor;

struct Tile {
    SDL_Texture* texture;
    int x, y, w, h;
};

using Resource = std::variant<SDL_Texture*, Tile, SDL_Cursor*>;

class ResourceManager {
public:
    struct TileDef {
        std::string name;
        int x, y, w = 1, h = 1;
    };

    ~ResourceManager();

    void add_texture(std::string const& name, std::vector<uint8_t> const& data);
    void add_tile(std::string const& name, std::string const& texture, int x, int y, int w, int h);
    void add_tiles(std::string const& parent_resource, std::vector<TileDef> const& tiles, int tile_size);
    void add_cursor(std::string const& name, SDL_Cursor* cursor);

    template <b::embed_string_literal identifier>
    void add_texture(std::string const& name) { add_texture(name, b::embed<identifier>().vec()); }

    void cleanup();

    [[nodiscard]] Resource     get(std::string const& name) const;
    [[nodiscard]] SDL_Texture* texture(std::string const& name) const { return find_resource<SDL_Texture*>(name); }
    [[nodiscard]] Tile         tile(std::string const& name) const { return find_resource<Tile>(name); }
    [[nodiscard]] SDL_Cursor*  cursor(std::string const& name) const { return find_resource<SDL_Cursor*>(name); }

private:
    ResourceManager() = default;

    template <typename T>
    T find_resource(std::string const& name) const {
        auto it = resources_.find(name);
        if (it == resources_.end())
            throw std::runtime_error("Resource " + name + " not found");
        if (!std::holds_alternative<SDL_Texture*>(it->second))
            throw std::runtime_error("Resource " + name + " not of the correct type");
        return std::get<T>(it->second);
    }

    std::unordered_map<std::string, Resource> resources_;

    friend ResourceManager& res();
};

ResourceManager& res();

#endif //RESOURCEMANAGER_HH
