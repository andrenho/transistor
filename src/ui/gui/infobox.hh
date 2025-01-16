#ifndef INFOBOX_HH
#define INFOBOX_HH

#include <string>
#include <variant>
#include <vector>

struct InfoboxContents {
    using Type = std::variant<std::string>;

    std::vector<Type> contents;

    InfoboxContents() = default;
    InfoboxContents(std::vector<Type> const& _contents) : contents(_contents) {}
    InfoboxContents(Type const& item) : contents({ item }) {}
    InfoboxContents(const char* item) : contents({ std::string(item) }) {}
};

void render_infobox(InfoboxContents const& contents);

#endif //INFOBOX_HH
