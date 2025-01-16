#ifndef UISTATE_HH
#define UISTATE_HH

#include <optional>
#include <string>

#include "ui_commands.hh"

struct UIState {
    std::string textbox = "";
    std::optional<U::ShowException> exception {};
};

#endif //UISTATE_HH
