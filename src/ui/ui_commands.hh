#ifndef UI_COMMANDS_HH
#define UI_COMMANDS_HH

#include <variant>

#include "engine/game/game.hh"


namespace ui {

using Command = std::variant<int>;

}

#endif //UI_COMMANDS_HH
