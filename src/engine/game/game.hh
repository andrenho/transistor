#ifndef GAME_HH
#define GAME_HH

#include "engine/sandbox/sandbox.hh"

class Game {
public:
    [[nodiscard]] Sandbox const& sandbox() const { return sandbox_; }
    [[nodiscard]] Sandbox& sandbox() { return sandbox_; }

    void update();

private:
    Sandbox sandbox_;
};



#endif //GAME_HH
