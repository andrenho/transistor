#include "game.hh"

#include <chrono>

#include "gamefilemanager.hh"
#include "engine/board/board.hh"
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
    ++update_count_;

    auto start = hr::now();
    execute_queue();
    while (hr::now() < start + 6ms)   // simulate for 6 ms
        sandbox_->simulate();

    if (update_count_ % 300 == 0)
        *this << G::Save {};
}

void Game::operator<<(G::Command&& command) const
{
    commands_.emplace(command);
}

void Game::execute(G::Command&& command)
{
    commands_.emplace(command);
    execute_queue();
}

void Game::execute_queue()
{
    while (!commands_.empty()) {

        std::visit(overloaded {

            [&](G::Save const&) {
                GameFileManager::save(*this);
            },
            [&](G::TryLoad const& cmd) {
                GameFileManager::try_load(*this, cmd.validate_version);
                sandbox_->recompile();
            },

            [&](G::StartPlacingWire const& cmd) {
                board(cmd.pos.board_id).start_placing_wire(cmd.width, cmd.layer, cmd.pos.x, cmd.pos.y);
            },
            [&](G::ContinuePlacingWire const& cmd) {
                board(cmd.pos.board_id).continue_placing_wire(cmd.pos.x, cmd.pos.y);
            },
            [&](G::FinishPlacingWire const& cmd) {
                board(cmd.pos.board_id).finish_placing_wire(cmd.pos.x, cmd.pos.y);
                sandbox_->recompile();
            },

            [&](G::AddComponent const& cmd) {
                board(cmd.pos.board_id).add_component(cmd.component_type, cmd.pos.x, cmd.pos.y);
                if (!cmd.bypass_recompilation)
                    sandbox_->recompile();
            },
            [&](G::RotateComponent const& cmd) {
                board(cmd.pos.board_id).rotate_component(cmd.pos.x, cmd.pos.y);
                sandbox_->recompile();
            },
            [&](G::ComponentClick const& cmd) {
                const_cast<Component *>(cmd.component)->on_click();
            },
            [&](G::ClearTile const& cmd) {
                board(cmd.pos.board_id).clear_tile(cmd.pos.x, cmd.pos.y);
                sandbox_->recompile();
            },

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

Game const& game()
{
    static const Game game(0);
    return game;
}

void game_update()
{
    const_cast<Game*>(&game())->update();
}

