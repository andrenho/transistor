#ifndef GAME_HH
#define GAME_HH

#include <memory>

#include "engine/sandbox/sandbox.hh"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Game {
public:
    [[nodiscard]] Sandbox const& sandbox() const { return *sandbox_.get(); }
    [[nodiscard]] Sandbox& sandbox() { return *sandbox_.get(); }

    void update();

    bool has_savename() const;
    void save(const char* filename=nullptr);
    void load(const char* filename);

    [[nodiscard]] json serialize() const;
    void unserialize(json const& content, bool validate_version);

private:
    std::unique_ptr<Sandbox> sandbox_ = std::make_unique<Sandbox>();
};



#endif //GAME_HH
