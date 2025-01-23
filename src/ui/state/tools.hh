#ifndef TOOLS_HH
#define TOOLS_HH

#include "res/resourcemanager.hh"

class Tools {
public:
    enum class Type {
        Nothing, VCC, Button, LED, NPN, PNP, Separator, Series,
    };

    struct Tool {
        Type           tool;
        resource_idx_t image;
        std::string    tooltip = "";
        std::string    infobox {};
    };

    struct Separator {};

    struct Category {
        std::string name;
        std::optional<resource_idx_t> image;
        std::vector<std::variant<Tool, Separator, Category>> children;
    };

    using ToolboxItem = std::variant<Tool, Separator, Category>;

    void init();

    [[nodiscard]] std::vector<ToolboxItem> const& all() const { return tools_; }
    [[nodiscard]] std::vector<Tool const*> const& toplevel() const { return tools_toplevel_; }

private:
    std::vector<ToolboxItem> tools_;
    std::vector<Tool const*> tools_toplevel_;

    resource_idx_t
        tb_arrow = -1, tb_vcc = -1, tb_button = -1, tb_led = -1, tb_npn = -1, tb_pnp = -1,
        tb_logic_gates = -1;
};

Tools const& tools();
void         tools_init();

#endif //TOOLS_HH
