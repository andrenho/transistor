#ifndef COMPONENTDATABASE_HH
#define COMPONENTDATABASE_HH

#include <memory>
#include <vector>

#include "componentdef.hh"

class ComponentDatabase {
public:
    [[nodiscard]] std::vector<std::unique_ptr<ComponentDefinition>> const& components() const;
};

#endif //COMPONENTDATABASE_HH
