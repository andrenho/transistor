#include "ui.hh"

UI::UI()
{

}

bool UI::running() const
{
    return true;
}

std::vector<Event> UI::events() const
{
    return {};
}

void UI::render(Render const& render) const
{

}
