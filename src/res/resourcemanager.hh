#ifndef RESOURCEMANAGER_HH
#define RESOURCEMANAGER_HH

#include <SDL_render.h>
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

using resource_idx_t = size_t;
using Resource = std::variant<SDL_Texture*, Tile, SDL_Cursor*>;
using ResourceId = std::variant<std::string, resource_idx_t>;

class ResourceManager {
public:
    struct TileDefName {
        std::string name;
        int x, y, w = 1, h = 1;
    };

    struct TileDefIdx {
        resource_idx_t* idx;
        int x, y, w = 1, h = 1;
    };

    ~ResourceManager();

    resource_idx_t add_texture(std::vector<uint8_t> const& data);

    void add_texture(std::string const& name, std::vector<uint8_t> const& data);

    resource_idx_t add_tile(ResourceId const& parent, int x, int y, int w, int h=0);
    void           add_tile(std::string const& name, ResourceId const& parent, int x, int y, int w, int h=0);

    void           add_tiles(ResourceId const& parent_resource, std::vector<TileDefName> const& tiles, int tile_size);
    void           add_tiles(ResourceId const& parent_resource, std::vector<TileDefIdx> const& tiles, int tile_size);

    resource_idx_t add_cursor(SDL_Cursor* cursor);
    void           add_cursor(std::string const& name, SDL_Cursor* cursor);

    template <b::embed_string_literal identifier>
    void add_texture(std::string const& name) { add_texture(name, b::embed<identifier>().vec()); }

    template <b::embed_string_literal identifier>
    resource_idx_t add_texture() { return add_texture(b::embed<identifier>().vec()); }

    void cleanup();

    [[nodiscard]] Resource     get(ResourceId const& r) const;
    [[nodiscard]] SDL_Texture* texture(ResourceId const& r) const { return convert_resource<SDL_Texture*>(get(r)); }
    [[nodiscard]] Tile         tile(ResourceId const& r) const { return convert_resource<Tile>(get(r)); }
    [[nodiscard]] SDL_Cursor*  cursor(ResourceId const& r) const {  return convert_resource<SDL_Cursor*>(get(r)); }

private:
    ResourceManager() = default;

    SDL_Texture* create_texture(std::vector<uint8_t> const& data);

    template <typename T>
    T convert_resource(Resource const& res) const {
        if (!std::holds_alternative<T>(res))
            throw std::runtime_error("Resource not of the correct type");
        return std::get<T>(res);
    }

    std::unordered_map<std::string, Resource> resources_str_;
    std::vector<Resource> resources_idx_;

    friend ResourceManager& res();
};

ResourceManager& res();

#endif //RESOURCEMANAGER_HH
