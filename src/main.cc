#include "ui/ui.hh"

#include <chrono>
#include <iostream>
using namespace std::chrono_literals;

#include "engine/game/game.hh"

int main()
{
    UI ui;
    Game game(0);
    game.enqueue(game::TryLoad {});

    auto last_frame = hr::now();

    while (ui.running()) {

        try {

            auto new_frame = hr::now();
            ui.update(game, new_frame - last_frame);
            game.update();

            ui.render(game);

            last_frame = new_frame;

        } catch (std::exception& e) {
            ui.report_exception(game, e);
        }

    }
}
