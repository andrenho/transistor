#include "ui/ui.hh"

#include <chrono>
using namespace std::chrono_literals;

#include "engine/sandbox/sandbox.hh"

int main()
{
    Sandbox sandbox;

    UI ui;
    ui.set_sandbox(sandbox);

    auto last_frame = hr::now();
    while (ui.running()) {
        auto new_frame = hr::now();
        while (hr::now() < new_frame + 6ms)   // simulate for 6 ms
            sandbox.simulate();
        ui.update(new_frame - last_frame);
        ui.render();
        last_frame = new_frame;
    }
}
