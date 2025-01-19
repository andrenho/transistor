#ifndef INFOBOX_HH
#define INFOBOX_HH

#include <string>

class Infobox {
public:
    void init();
    void render() const;

private:
    void render_contents(std::string const& contents) const;

    void render_tag(std::string const& type, std::string const& text) const;
};

#endif //INFOBOX_HH
