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

    int x, y, w, h;
    float zoom = 1.f;

protected:
    UILayer(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
};

#endif //LAYER_HH
