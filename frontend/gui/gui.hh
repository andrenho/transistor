#ifndef GUI_HH
#define GUI_HH

#include "render.hh"

class GUI {
public:
    ~GUI();

    void init();
    void render(Render const& render);
};

#endif //GUI_HH
