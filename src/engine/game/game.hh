#ifndef GAME_HH
#define GAME_HH

#include <memory>

#include "engine/sandbox/sandbox.hh"

#include <nlohmann/json.hpp>

#include "game_commands.hh"
using json = nlohmann::json;

class Game {
public:
    void operator<<(G::Command&& command) const;

    void execute(G::Command&& command);
    void update();

    [[nodiscard]] json serialize() const;
    void unserialize(json const& content, bool validate_version);

    [[nodiscard]] Sandbox const& sandbox() const { return *sandbox_.get(); }
    [[nodiscard]] Sandbox& sandbox() { return *sandbox_.get(); }

    [[nodiscard]] Board const& board(size_t id) const { return sandbox_->board(id); }
    [[nodiscard]] Board& board(size_t id) { return sandbox_->board(id); }

    [[nodiscard]] gameid_t id() const { return id_; }

    [[nodiscard]] std::optional<std::string> infobox_text(std::string const& component_name) const;

private:
    explicit Game(gameid_t id);

    void execute_queue();

    gameid_t id_;
    size_t update_count_ = 0;
    mutable std::queue<G::Command> commands_;
    std::unique_ptr<Sandbox> sandbox_ = std::make_unique<Sandbox>();

    friend Game const& game(gameid_t);
};

Game const& game(gameid_t game_id=0);
void        game_update();

#endif //GAME_HH
