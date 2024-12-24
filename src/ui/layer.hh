#ifndef LAYER_HH
#define LAYER_HH

#include <functional>

#include "resources/resource.hh"

struct DrawProperties {
};

using DrawF = std::function<void(Resource const&, int x, int y, DrawProperties const& dp)>;

class Layer {
public:
    virtual ~Layer() = default;

    virtual void render(DrawF draw);

    int layer_x, layer_y, layer_w, layer_h;

protected:
    Layer(int x, int y, int w, int h) : layer_x(x), layer_y(y), layer_w(w), layer_h(h) {}
};

#endif //LAYER_HH
