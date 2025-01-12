#ifndef GAME_HH
#define GAME_HH

#include <memory>

#include "engine/sandbox/sandbox.hh"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Game {
public:
    explicit Game(gameid_t id);

    [[nodiscard]] Sandbox const& sandbox() const { return *sandbox_.get(); }
    [[nodiscard]] Sandbox& sandbox() { return *sandbox_.get(); }

    void update();

    void save() const;
    void try_load(bool validate_version=true);

    [[nodiscard]] json serialize() const;
    void unserialize(json const& content, bool validate_version);

private:
    gameid_t id_;
    std::unique_ptr<Sandbox> sandbox_ = std::make_unique<Sandbox>();

    static std::string game_path();
};



#endif //GAME_HH
