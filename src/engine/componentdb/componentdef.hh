#ifndef COMPONENTDEFINITION_HH
#define COMPONENTDEFINITION_HH

#include <string>

struct ComponentDefinition {
    enum class Type { SingleTile, IC };

    std::string name;
    Type        type;
    bool        can_rotate;

    size_t      data_size = 0;

    // TODO - add IC fields (size, pins)
};

#endif //COMPONENTDEFINITION_HH
