#ifndef DIALOG_HH
#define DIALOG_HH

#include <functional>
#include <string>
#include <vector>

extern "C" {
#include <transistor-sandbox.h>
}

struct DialogButton {
    std::string                           text;
    std::function<void(ts_Transistor* t)> function;
};

class Dialog {
public:
    Dialog(std::string const& title, std::vector<std::string> const& text, std::vector<DialogButton> const& buttons)
        : title_(title), text_(text), buttons_(buttons) {}

    void reset() { show_ = false; }
    void show() { show_ = true; }
    void render(ts_Transistor* T) const;

private:
    std::string               title_;
    std::vector<std::string>  text_;
    std::vector<DialogButton> buttons_;
    bool                      show_ = false;
};

class YesNoDialog : public Dialog {
public:
    YesNoDialog(std::string const& title, std::vector<std::string> const& text, std::function<void(ts_Transistor* t)> on_yes)
        : Dialog(title, text, { { "Yes", on_yes }, { "No", [this](ts_Transistor*){} } }) {}
};

class MessageBox : public Dialog {
public:
    MessageBox(std::string const& title, std::vector<std::string> const& text)
        : Dialog(title, text, { { "Ok", [this](ts_Transistor*) {} } }) {}
};

#endif //DIALOG_HH
