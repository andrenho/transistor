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
};

#endif //WIRE_HH
