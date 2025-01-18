#ifndef INFOBOX_HH
#define INFOBOX_HH

class Infobox {
public:
    void init();
    void render() const;

private:
    void render_contents() const;
};

#endif //INFOBOX_HH
