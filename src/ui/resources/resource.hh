#ifndef RESOURCE_HH
#define RESOURCE_HH

#include <variant>

struct SDL_Texture;

struct Resource {
    struct SubTexture {
        SDL_Texture* texture;
        int x, y, w, h;
    };
    using InternalType = std::variant<SDL_Texture*, SubTexture>;

    Resource() = default;
    explicit Resource(InternalType const& data_) : data(data_) {}

    operator SDL_Texture*() const { return std::get<SDL_Texture*>(data); }
    operator SubTexture() const { return std::get<SubTexture>(data); }

    bool is_texture() const { return std::holds_alternative<SDL_Texture*>(data); }
    bool is_subtexture() const { return std::holds_alternative<SubTexture>(data); }

    InternalType data;
};

#endif //RESOURCE_HH
