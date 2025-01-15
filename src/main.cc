#include "ui/ui.hh"

#include <chrono>
#include <iostream>
using namespace std::chrono_literals;

#include "engine/game/game.hh"

int main()
{
    Game game(0);
    game << game::TryLoad {};

    UI ui;
    ui << ui::Reset { game };

    auto last_frame = hr::now();

    while (ui.running()) {

        try {

            auto new_frame = hr::now();

            game.update();
            ui.update(game, new_frame - last_frame);

            ui.render(game);

            last_frame = new_frame;

        } catch (std::exception& e) {
            ui.report_exception(game, e);
        }

    }
}
