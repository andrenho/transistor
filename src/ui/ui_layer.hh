#ifndef LAYER_HH
#define LAYER_HH

#include "uiinterface.hh"

class UILayer {
public:
    virtual ~UILayer() = default;

    virtual void render(UI_Interface const& uif) = 0;

    int x, y, w, h;
    float zoom = 1.f;

protected:
    UILayer(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
};

#endif //LAYER_HH
