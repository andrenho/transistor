#ifndef GAME_COMMANDS_HH
#define GAME_COMMANDS_HH

#include <string>
#include <variant>

namespace game {

struct Save {};
struct TryLoad { bool validate_version = true; };

struct StartPlacingWire {
    Wire::Width width;
    Wire::Layer layer;
    Position    pos;
};
struct ContinuePlacingWire { Position pos; };
struct FinishPlacingWire { Position pos; };

struct AddComponent {
    std::string component_type;
    Position    pos;
};

struct RotateComponent { Position pos; };

struct ComponentClick { Component const* component; };

struct ClearTile { Position pos; };

using Command = std::variant<
    Save, TryLoad,
    StartPlacingWire, ContinuePlacingWire, FinishPlacingWire,
    AddComponent, RotateComponent, ComponentClick,
    ClearTile
>;

}

#endif //GAME_COMMANDS_HH
