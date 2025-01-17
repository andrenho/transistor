#ifndef RESOURCEMANAGER_HH
#define RESOURCEMANAGER_HH

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct SDL_Texture;
struct SDL_Cursor;

class ResourceManager {
public:
    struct Tile {
        SDL_Texture* texture;
        int x, y, w, h;
    };

    struct TileDef {
        std::string name;
        int x, y, w, h;
    };

    ~ResourceManager();

    void add_texture(std::string const& name, std::vector<uint8_t> const& data);
    void add_tile(std::string const& name, std::string const& texture, int x, int y, int w, int h);
    void add_tiles(std::string const& name, std::vector<TileDef> const& tiles);
    void add_cursor(std::string const& name, SDL_Cursor* cursor);

    void cleanup();

    [[nodiscard]] SDL_Texture* texture(std::string const& name) const { return find_resource<SDL_Texture*>(name); }
    [[nodiscard]] Tile         tile(std::string const& name) const { return find_resource<Tile>(name); }
    [[nodiscard]] SDL_Cursor*  cursor(std::string const& name) const { return find_resource<SDL_Cursor*>(name); }

private:
    ResourceManager() = default;

    using Resource = std::variant<SDL_Texture*, Tile, SDL_Cursor*>;

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
