#include "game.hh"

#include <chrono>

#include "util/exceptions.hh"
using namespace std::chrono_literals;
using hr = std::chrono::high_resolution_clock;

#include "../sandbox/sandbox.hh"

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

bool Game::has_savename() const
{
    return false;
}

void Game::save(const char* filename)
{

}

void Game::load(const char* filename)
{
}
