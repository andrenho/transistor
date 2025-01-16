#ifndef UISTATE_HH
#define UISTATE_HH

#include <optional>
#include <string>

struct UIState {
    std::string textbox = "";
    std::optional<std::string> exception_text;
};

#endif //UISTATE_HH
