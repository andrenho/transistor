#ifndef UI_COMMANDS_HH
#define UI_COMMANDS_HH

#include <variant>

#include "engine/game/game.hh"


namespace U {

struct Quit {};

using Command = std::variant<Quit>;

}

#endif //UI_COMMANDS_HH
