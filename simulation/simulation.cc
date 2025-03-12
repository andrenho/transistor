#include "simulation.hh"

#include <cassert>
#include <lua.hpp>

#include <chrono>
#include <string>
using namespace std::chrono_literals;
using namespace std::string_literals;

Simulation::Simulation(lua_State* L)
    : L(L)
{
}

void Simulation::start()
{
    if (thread_.joinable())
        return;

    running_ = true;
    paused_ = false;
    thread_ = std::thread(simulation_thread, this);
}

void Simulation::stop()
{
    if (thread_.joinable()) {
        resume();
        running_ = false;
        thread_.join();
    }
}

void Simulation::simulation_thread(Simulation* simulation)
{
    while (simulation->running_) {

        {
            // pause thread
            std::unique_lock lock(simulation->mutex_);
            simulation->cv_.wait(lock, [simulation] { return !simulation->paused_; });

            // execute
            simulation_single_step(simulation);
        }

        // give the CPU a break
        switch (simulation->cpu_usage_) {
            case CpuUsage::Light:      std::this_thread::sleep_for(100ns); break;
            case CpuUsage::Normal:     std::this_thread::yield(); break;
            case CpuUsage::Aggressive: break;
        }

        // TODO - calculate timing
    }
}

void Simulation::simulation_single_step(Simulation* simulation)
{
    assert(lua_gettop(simulation->L) == 0);

    // simulate components (C)
    for (auto& component: simulation->result_.components)
        component.simulate(component.data, component.pins);

    // simulate components (Lua)
    if (simulation->simulate_luaref_ != -1) {
        lua_rawgeti(simulation->L, LUA_REGISTRYINDEX, simulation->simulate_luaref_);
        if (lua_pcall(simulation->L, 0, 0, 0) != LUA_OK)
            throw std::runtime_error("Error executing Lua simulation: "s + lua_tostring(simulation->L, -1));
    }

    // update connection and pin values
    for (auto& connection: simulation->result_.connections) {

        // calculate value from output pins
        connection.value = 0;
        for (auto const& pin: connection.pins)
            if (pin.dir == PinDirection::Output)
                connection.value |= pin.pins[pin.pin_no];

        // set input pins
        for (auto const& pin: connection.pins)
            if (pin.dir == PinDirection::Input)
                pin.pins[pin.pin_no] = connection.value;
    }

    ++simulation->steps_;
}

void Simulation::update_compilation_result(CompilationResult&& result)
{
    result_ = std::move(result);
}

void Simulation::pause()
{
    std::lock_guard lock(mutex_);
    paused_ = true;
}

void Simulation::resume()
{
    {
        std::lock_guard lock(mutex_);
        paused_ = false;
    }
    cv_.notify_one();
}

