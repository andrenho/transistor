#ifndef LAYER_HH
#define LAYER_HH

#include "uiinterface.hh"

class UILayer {
public:
    virtual ~UILayer() = default;

    virtual void render() = 0;

    virtual void on_mouse_press(int x, int y, uint8_t button, bool dbl_click) {}
    virtual void on_mouse_release(int x, int y, uint8_t button) {}
    virtual void on_mouse_move(int x, int y, int rx, int ry) {}
    virtual void on_key_press(uint32_t key, int x, int y) {}
    virtual void on_key_release(uint32_t key, int x, int y) {}

    virtual void set_x(int pos_x) { x_ = pos_x; }
    virtual void set_y(int pos_y) { y_ = pos_y; }

    [[nodiscard]] virtual int x() const { return x_; }
    [[nodiscard]] virtual int y() const { return y_; }
    [[nodiscard]] virtual int w() const { return w_; }
    [[nodiscard]] virtual int h() const { return h_; }
    [[nodiscard]] virtual float zoom() const { return zoom_; }

protected:
    UILayer(UI_Interface& uif, int x, int y, int w, int h) : uif_(uif), x_(x), y_(y), w_(w), h_(h) {}

    UI_Interface& uif_;
    int x_, y_, w_, h_;
    float zoom_ = 1.f;
};

#endif //LAYER_HH
