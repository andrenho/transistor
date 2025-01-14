#include "gamefilemanager.hh"

#include <fstream>
#include <ostream>
#include <stdexcept>
#include <sys/stat.h>

#include "game.hh"

void GameFileManager::init()
{
    mkdir(game_path().c_str(), 0755);
}

void GameFileManager::save(Game const& game)
{
    std::ofstream o(game_path() + "game" + std::to_string(game.id()) + ".json");
    o << game.serialize() << std::endl;
}

void GameFileManager::try_load(Game& game, bool validate_version)
{
    std::ifstream i(game_path() + "game" + std::to_string(game.id()) + ".json");
    if (i.good()) {
        json content;
        i >> content;
        game.unserialize(content, validate_version);
    }
}

std::string GameFileManager::game_path()
{
    const char* home = std::getenv("HOME");
    const char* user_profile = std::getenv("USERPROFILE");

    if (home)
        return std::string(home) + "/.config/transistor/";
    if (user_profile)
        return std::string(user_profile) + "\\transistor\\";

    throw std::runtime_error("Couldn't file save directory.");
}