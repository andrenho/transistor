#ifndef INFOBOX_HH
#define INFOBOX_HH

#include <string>
#include <variant>
#include <vector>

struct InfoboxContents {
    using Type = std::variant<std::string>;

    std::vector<Type> contents;

    InfoboxContents() = default;
    InfoboxContents(Type const& item) : contents({ item }) {}
    InfoboxContents(const char* item) : contents({ std::string(item) }) {}
    InfoboxContents(std::initializer_list<Type> c) : contents(c) {}
};

void render_infobox(InfoboxContents const& contents);

#endif //INFOBOX_HH
