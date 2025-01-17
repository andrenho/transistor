#ifndef TOOLBOX_HH
#define TOOLBOX_HH
#include "res/resourcemanager.hh"

class Toolbox {
public:
    void init();
    void render() const;

private:
    int icons_tx_w = 0, icons_tx_h = 0;
    resource_idx_t tb_arrow = -1;

    bool image_button(ResourceId const& resource) const;
};

#endif //TOOLBOX_HH
