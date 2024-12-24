#ifndef LAYER_HH
#define LAYER_HH

#include <functional>

#include "resources/resource.hh"

struct DrawProperties {
};

using DrawF = std::function<void(Resource const& res, int x, int y, DrawProperties const& dp)>;

class UILayer {
public:
    virtual ~UILayer() = default;

    virtual void render(DrawF draw) = 0;

    int layer_x, layer_y, layer_w, layer_h;

protected:
    UILayer(int x, int y, int w, int h) : layer_x(x), layer_y(y), layer_w(w), layer_h(h) {}
};

#endif //LAYER_HH
