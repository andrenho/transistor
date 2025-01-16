#ifndef UISTATE_HH
#define UISTATE_HH

#include <optional>
#include <string>

#include "ui_commands.hh"
#include "gui/infobox.hh"

struct UIState {
    InfoboxContents                 infobox_contents;
    std::optional<U::ShowException> exception {};
};

#endif //UISTATE_HH
