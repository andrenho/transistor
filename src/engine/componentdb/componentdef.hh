#ifndef COMPONENTDEFINITION_HH
#define COMPONENTDEFINITION_HH


struct ComponentDefinition {
    enum class Type { SingleTile, IC };

    Type type;
    bool can_rotate;

    // TODO - add IC fields (size, pins)
};

#endif //COMPONENTDEFINITION_HH
