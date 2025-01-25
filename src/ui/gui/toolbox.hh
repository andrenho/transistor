#ifndef TOOLBOX_HH
#define TOOLBOX_HH

#include "engine/componentdb/componentdef.hh"
#include "res/resourcemanager.hh"

class Toolbox {
public:
    void init();
    void render() const;

private:
    int icons_tx_w = 0, icons_tx_h = 0;
    resource_idx_t tb_arrow = -1, tb_vcc = -1, tb_button = -1, tb_led = -1, tb_npn = -1, tb_pnp = -1,
                   tb_logic_gates = -1;

    bool image_button(ResourceId const& resource, size_t i) const;

    struct Button {
        resource_idx_t*  image;
        std::string      component_name;
        std::string      tooltip;
    };
    std::vector<Button> buttons_;
};

#endif //TOOLBOX_HH
