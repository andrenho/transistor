#ifndef LAYER_HH
#define LAYER_HH

#include "../scene.hh"
#include "ui/events.hh"

class Layer : public Scene::ImageContext {
public:
    ~Layer() override = default;

    virtual void render(Scene& scene) = 0;

    virtual void on_mouse_press(int x, int y, uint8_t button, bool dbl_click, Events& events) {}
    virtual void on_mouse_release(int x, int y, uint8_t button, Events& events) {}
    virtual void on_mouse_move(int x, int y, int rx, int ry, Events& events) {}
    virtual void on_key_press(uint32_t key, int x, int y, Events& events) {}
    virtual void on_key_release(uint32_t key, int x, int y, Events& events) {}

    virtual void set_x(int pos_x) { x_ = pos_x; }
    virtual void set_y(int pos_y) { y_ = pos_y; }

    [[nodiscard]] int x() const override { return x_; }
    [[nodiscard]] int y() const override { return y_; }
    [[nodiscard]] float zoom() const override { return zoom_; }

    [[nodiscard]] virtual int w() const { return w_; }
    [[nodiscard]] virtual int h() const { return h_; }

protected:
    Layer(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h) {}

    int x_, y_, w_, h_;
    float zoom_ = 1.f;
};

#endif //LAYER_HH
