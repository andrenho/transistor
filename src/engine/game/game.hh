#ifndef GAME_HH
#define GAME_HH

#include <memory>

#include "engine/sandbox/sandbox.hh"

#include <nlohmann/json.hpp>

#include "game_commands.hh"
using json = nlohmann::json;

class Game {
public:
    explicit Game(gameid_t id);

    [[nodiscard]] Sandbox const& sandbox() const { return *sandbox_.get(); }
    [[nodiscard]] Sandbox& sandbox() { return *sandbox_.get(); }

    void enqueue(game::Command&& command) const;
    void enqueue_and_execute(game::Command&& command);
    void update();

    [[nodiscard]] json serialize() const;
    void unserialize(json const& content, bool validate_version);

    [[nodiscard]] gameid_t id() const { return id_; }

private:
    void execute_queue();

    gameid_t id_;
    mutable std::queue<game::Command> commands_;
    std::unique_ptr<Sandbox> sandbox_ = std::make_unique<Sandbox>();
};



#endif //GAME_HH
