#include "infobox.hh"

#include "imgui.h"
#include "util/visitor.hh"

void render_infobox(InfoboxContents const& contents)
{
    for (auto const& item: contents) {
        std::visit(overloaded {
            [](std::string const& str) { ImGui::Text("%s", str.c_str()); },
        }, item);
    }
}
