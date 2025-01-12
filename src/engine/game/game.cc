#include "game.hh"

#include <chrono>
using namespace std::chrono_literals;
using hr = std::chrono::high_resolution_clock;

#include "../sandbox/sandbox.hh"

void Game::update()
{
    auto start = hr::now();
    while (hr::now() < start + 6ms)   // simulate for 6 ms
        sandbox_.simulate();
}
