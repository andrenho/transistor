#ifndef UISTATE_HH
#define UISTATE_HH

#include <optional>

#include "ui_commands.hh"
#include "gui/infobox.hh"
#include "gui/toolbox.hh"

struct UIState {
    InfoboxContents                 infobox_contents;
    std::optional<U::ShowException> exception {};
    Toolbox                         toolbox;
};

#endif //UISTATE_HH
