#ifndef LAYER_HH
#define LAYER_HH

#include "uiinterface.hh"

class UILayer {
public:
    virtual ~UILayer() = default;

    virtual void render(UI_Interface const& uif) = 0;

    virtual void on_mouse_press(int x, int y, uint8_t button, bool dbl_click) {}
    virtual void on_mouse_release(int x, int y, uint8_t button) {}
    virtual void on_mouse_move(int x, int y, int rx, int ry) {}
    virtual void on_key_press(uint32_t key, int x, int y) {}
    virtual void on_key_release(uint32_t key, int x, int y) {}

    int x, y, w, h;
    float zoom = 1.f;

protected:
    UILayer(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
};

#endif //LAYER_HH
