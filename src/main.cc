#include "ui/ui.hh"

#include <chrono>
using namespace std::chrono_literals;

#include "engine/game/game.hh"

int main()
{
    Game game(0);
    game.try_load();

    UI ui;
    ui.set_game(game);

    auto last_frame = hr::now();
    while (ui.running()) {
        auto new_frame = hr::now();
        game.update();
        ui.update(new_frame - last_frame);
        ui.render();
        last_frame = new_frame;
    }
}
