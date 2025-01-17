#include "ui/ui.hh"

#include <chrono>
#include <iostream>

#include "util/exceptions.hh"
using namespace std::chrono_literals;

#include "engine/game/game.hh"

int main()
{
    auto last_frame = hr::now();

    game(0) << G::TryLoad {};

    ui_init();

    while (ui().running()) {

        try {
            auto new_frame = hr::now();

            game_update();
            ui_update(new_frame - last_frame);

            ui().render();

            last_frame = new_frame;

        } catch (RecoverableException& e) {
            ui() << U::ShowException { e.what(), true };

        } catch (std::exception& e) {
            ui() << U::ShowException { e.what() };
        }
    }
}
