#ifndef WIRE_HH
#define WIRE_HH

#include <array>
#include <cstdint>

struct Wire {
    enum class Width : uint8_t { W1 = '1' };
    enum class Layer : uint8_t { Top = 't', Bottom = 'b' };

    Width width;
    Layer layer;

    static constexpr auto ALL_WIDTHS = { Width::W1 };
    static constexpr auto ALL_LAYERS = { Layer::Top, Layer::Bottom };

    friend bool operator==(Wire const& lhs, Wire const& rhs) { return lhs.width == rhs.width && lhs.layer == rhs.layer; }
    friend bool operator!=(Wire const& lhs, Wire const& rhs) { return !(lhs == rhs); }
};

template <>
struct std::hash<Wire> {
    std::size_t operator()(Wire const& wire) const noexcept {
        return (((uint8_t) wire.layer) << 8) | (uint8_t) wire.width;
    }
};

#endif //WIRE_HH
