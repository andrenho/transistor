#include "game.hh"

#include <sys/stat.h>

#include <chrono>
#include <fstream>
#include <istream>
#include <ostream>

#include "util/exceptions.hh"
using namespace std::chrono_literals;
using hr = std::chrono::high_resolution_clock;

#include "../sandbox/sandbox.hh"

Game::Game(gameid_t id)
    : id_(id)
{
    mkdir(game_path().c_str(), 0755);
}

void Game::update()
{
    auto start = hr::now();
    while (hr::now() < start + 6ms)   // simulate for 6 ms
        sandbox_->simulate();
}

json Game::serialize() const
{
    return json {
        { "sandbox", sandbox_->serialize() },
    };
}

void Game::unserialize(json const& content, bool validate_version)
{
    if (validate_version) {
        if (content.at("version").at("major") > PROJECT_VERSION_MAJOR
                || (content.at("version").at("major") == PROJECT_VERSION_MAJOR && content.at("version").at("minor") > PROJECT_VERSION_MINOR)
                || (content.at("version").at("major") == PROJECT_VERSION_MAJOR && content.at("version").at("minor") == PROJECT_VERSION_MINOR && content.at("version").at("patch") > PROJECT_VERSION_PATCH))
            throw RecoverableErrorOptional("This file was created with a higher version than the current runtime, and it might present errors when loading. Do you want to continue?");
    }

    sandbox_ = std::make_unique<Sandbox>(content.at("sandbox"));
}

void Game::save() const
{
    std::ofstream o(game_path() + "game" + std::to_string(id_) + ".json");
    o << serialize() << std::endl;
}

void Game::try_load(bool validate_version)
{
    std::ifstream i(game_path() + "game" + std::to_string(id_) + ".json");
    if (i.good()) {
        json content;
        i >> content;
        unserialize(content, validate_version);
    }
}

std::string Game::game_path()
{
    const char* home = std::getenv("HOME");
    const char* user_profile = std::getenv("USERPROFILE");

    if (home)
        return std::string(home) + "/.config/transistor/";
    if (user_profile)
        return std::string(user_profile) + "\\transistor\\";

    throw std::runtime_error("Couldn't file save directory.");
}
