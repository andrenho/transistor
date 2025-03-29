#ifndef UI_HH
#define UI_HH

#include <vector>
#include "mappers/out/event.hh"
#include "mappers/in/render.hh"

class UI {
public:
    UI();

    [[nodiscard]] bool               running() const;
    [[nodiscard]] std::vector<Event> events() const;
    void                             render(Render const& render) const;
};

#endif //UI_HH
