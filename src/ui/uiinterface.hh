#ifndef UIINTERFACE_HH
#define UIINTERFACE_HH

struct DrawProperties {
    bool semitransparent = false;
};

class UILayer;

class UI_Interface {
public:
    virtual ~UI_Interface() = default;

    virtual void draw_image(UILayer const* layer, struct Resource const& res, int x, int y, DrawProperties const& dp={}) const = 0;

    virtual void start_dragging(UILayer* layer) = 0;
    virtual void stop_dragging() = 0;
};

#endif //UIINTERFACE_HH
