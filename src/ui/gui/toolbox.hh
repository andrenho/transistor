#ifndef TOOLBOX_HH
#define TOOLBOX_HH

#include "res/resourcemanager.hh"
#include "ui/uistate.hh"

class Toolbox {
public:
    void init();
    void render() const;

private:
    int icons_tx_w = 0, icons_tx_h = 0;
    resource_idx_t tb_arrow = -1, tb_vcc = -1, tb_button = -1, tb_led = -1, tb_npn = -1, tb_pnp = -1;

    bool image_button(ResourceId const& resource, size_t i) const;

    struct Button {
        SelectedTool          tool;
        resource_idx_t        image;
        std::string           tooltip;
        std::function<void()> cb = nullptr;
    };
    std::vector<Button> buttons_;
};

#endif //TOOLBOX_HH
