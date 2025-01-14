#include "ui/ui.hh"

#include <chrono>
using namespace std::chrono_literals;

#include "engine/game/game.hh"

int main()
{
    UI ui;
    Game game(0);
    bool setup = true;

    auto last_frame = hr::now();

    while (ui.running()) {

        try {

            if (setup) {
                game.enqueue(game::TryLoad {});
                ui.set_game(game);
                setup = false;
            }

            auto new_frame = hr::now();
            game.update();
            ui.update(new_frame - last_frame);
            ui.render();
            last_frame = new_frame;

        } catch (std::exception& e) {
            ui.report_exception(e);
        }

    }
}
