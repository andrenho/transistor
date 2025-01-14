#include "game.hh"

#include <chrono>

#include "gamefilemanager.hh"
#include "util/exceptions.hh"
using namespace std::chrono_literals;
using hr = std::chrono::high_resolution_clock;

#include "../sandbox/sandbox.hh"
#include "util/visitor.hh"

Game::Game(gameid_t id)
    : id_(id)
{
    GameFileManager::init();
}

void Game::update()
{
    auto start = hr::now();
    execute_queue();
    while (hr::now() < start + 6ms)   // simulate for 6 ms
        sandbox_->simulate();
}

void Game::enqueue(game::Command&& command) const
{
    commands_.emplace(command);
}


void Game::enqueue_and_execute(game::Command&& command) {
    enqueue(std::move(command));
    execute_queue();
}

void Game::execute_queue()
{
    while (!commands_.empty()) {

        std::visit(overloaded {
            [&](game::Save const&) { GameFileManager::save(*this); },
            [&](game::TryLoad const& load) { GameFileManager::try_load(*this, load.validate_version); },
        }, commands_.front());

        commands_.pop();
    }
}

json Game::serialize() const
{
    return json {
        { "sandbox", sandbox_->serialize() },
        { "version", json {
            { "major", PROJECT_VERSION_MAJOR },
            { "minor", PROJECT_VERSION_MINOR },
            { "patch", PROJECT_VERSION_PATCH },
        } }
    };
}

void Game::unserialize(json const& content, bool validate_version)
{
    if (validate_version) {
        if (content.at("version").at("major") > PROJECT_VERSION_MAJOR
                || (content.at("version").at("major") == PROJECT_VERSION_MAJOR && content.at("version").at("minor") > PROJECT_VERSION_MINOR)
                || (content.at("version").at("major") == PROJECT_VERSION_MAJOR && content.at("version").at("minor") == PROJECT_VERSION_MINOR && content.at("version").at("patch") > PROJECT_VERSION_PATCH))
            throw std::runtime_error("This file was created with a higher version than the current runtime.");
    }

    sandbox_ = std::make_unique<Sandbox>(content.at("sandbox"));
}