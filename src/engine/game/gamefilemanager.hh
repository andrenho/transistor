#ifndef GAMEFILEMANAGER_HH
#define GAMEFILEMANAGER_HH

#include <string>

class Game;

class GameFileManager {
public:
    static void init();

    static void save(Game const& game);
    static void try_load(Game& game, bool validate_version=true);

private:
    static std::string game_path();
};



#endif //GAMEFILEMANAGER_HH
