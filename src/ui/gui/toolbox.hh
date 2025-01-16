#ifndef TOOLBOX_HH
#define TOOLBOX_HH

#include <vector>

#include "ui/resources/resource.hh"

struct ToolboxButton {
    Resource image;
};

using Toolbox = std::vector<ToolboxButton>;

#endif //TOOLBOX_HH
