#ifndef UIINTERFACE_HH
#define UIINTERFACE_HH

struct DrawProperties {
};

class UI_Interface {
public:
    virtual ~UI_Interface() = default;

    virtual void draw_image(class UILayer const* layer, struct Resource const& res, int x, int y, DrawProperties const& dp={}) const = 0;
};

#endif //UIINTERFACE_HH
