#ifndef LAYER_HH
#define LAYER_HH

#include "uiinterface.hh"

class UILayer {
public:
    virtual ~UILayer() = default;

    virtual void render(UI_Interface const& uif) = 0;

    virtual void on_mouse_press(UI_Interface& uif, int x, int y, uint8_t button, bool dbl_click) {}
    virtual void on_mouse_release(UI_Interface& uif, int x, int y, uint8_t button) {}
    virtual void on_mouse_move(UI_Interface& uif, int x, int y, int rx, int ry) {}
    virtual void on_key_press(UI_Interface& uif, uint32_t key, int x, int y) {}
    virtual void on_key_release(UI_Interface& uif, uint32_t key, int x, int y) {}

    int pos_x, pos_y, w, h;
    float zoom = 1.f;

protected:
    UILayer(int x, int y, int w, int h) : pos_x(x), pos_y(y), w(w), h(h) {}
};

#endif //LAYER_HH
