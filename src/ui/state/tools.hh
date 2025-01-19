#ifndef TOOLS_HH
#define TOOLS_HH

#include "res/resourcemanager.hh"

enum class SelectedTool {
    Nothing, VCC, Button, LED, NPN, PNP,
};

struct Tool {
    SelectedTool   tool;
    resource_idx_t image;
    std::string    tooltip;
    std::string    infobox {};
};

void                     tools_init();
std::vector<Tool> const& tools();

#endif //TOOLS_HH
