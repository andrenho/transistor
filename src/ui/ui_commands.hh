#ifndef UI_COMMANDS_HH
#define UI_COMMANDS_HH

#include <string>
#include <variant>

#include "engine/game/game.hh"


namespace ui {

struct Reset { Game const& game; };

using Command = std::variant<Reset>;

}

#endif //UI_COMMANDS_HH
