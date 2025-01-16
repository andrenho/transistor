#include "ui/ui.hh"

#include <chrono>
#include <iostream>
using namespace std::chrono_literals;

#include "engine/game/game.hh"

int main()
{
    game(0) << G::TryLoad {};

    auto last_frame = hr::now();

    while (ui().running()) {

        try {

            auto new_frame = hr::now();

            game_update();
            ui_update(new_frame - last_frame);

            ui().render();

            last_frame = new_frame;

        } catch (std::exception& e) {

            ui() << U::ShowException { e };

        }

    }
}
