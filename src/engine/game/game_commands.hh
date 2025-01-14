#ifndef GAME_COMMANDS_HH
#define GAME_COMMANDS_HH

#include <variant>

namespace game {

struct Save {};
struct TryLoad { bool validate_version = true; };

using Command = std::variant<Save, TryLoad>;

}

#endif //GAME_COMMANDS_HH
