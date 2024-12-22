#ifndef WIRE_HH
#define WIRE_HH
#include <cstdint>

struct Wire {
    enum class Width : uint8_t { W1 = '1' };
    enum class Layer : uint8_t { Top = 't', Bottom = 'b' };

    Width width;
    Layer layer;
};

#endif //WIRE_HH
