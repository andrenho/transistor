#include "simulation.hh"

#include <cassert>
#include <lua.hpp>

#include <chrono>
#include <string>

#include "luamutex.hh"
using namespace std::chrono_literals;
using namespace std::string_literals;

void Simulation::start()
{
    if (thread_.joinable())
        return;

    running_ = true;
    thread_ = std::thread(simulation_thread, this);
}

void Simulation::stop()
{
    if (thread_.joinable()) {
        running_ = false;
        thread_.join();
    }
}

void Simulation::simulation_thread(Simulation* simulation)
{
    while (simulation->running_) {

        // execute
        lua.execute([&simulation](lua_State* L) {
            simulation_single_step(L, simulation);

            // give the CPU a break
            switch (simulation->cpu_usage_) {
                case CpuUsage::Light:      std::this_thread::sleep_for(100ns); break;
                case CpuUsage::Normal:     std::this_thread::yield(); break;
                case CpuUsage::Aggressive: break;
            }
        });

    }
}

void Simulation::simulation_single_step(lua_State* L, Simulation* simulation)
{
    assert(lua_gettop(L) == 0);

    // simulate components (C)
    for (auto& component: simulation->result_.components)
        if (component.simulate)
            component.simulate(component.data, component.pins);

    // simulate components (Lua)
    if (simulation->simulate_luaref_ != -1) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, simulation->simulate_luaref_);
        if (lua_pcall(L, 0, 0, 0) != LUA_OK)
            throw std::runtime_error("Error executing Lua simulation: "s + lua_tostring(L, -1));
    }

    // update connection and pin values
    for (auto& connection: simulation->result_.connections) {

        // calculate value from output pins
        connection.value = 0;
        for (auto const& pin: connection.pins)
            if (pin.dir == CompilationResult::PinDirection::Output)
                connection.value |= pin.pins[pin.pin_no];

        // set input pins
        for (auto const& pin: connection.pins)
            if (pin.dir == CompilationResult::PinDirection::Input)
                pin.pins[pin.pin_no] = connection.value;
    }

    ++simulation->steps_;
}

void Simulation::update_compilation_result(CompilationResult&& result)
{
    result_ = std::move(result);
}

void Simulation::reset_steps()
{
    lua.execute([this](lua_State*) { steps_ = 0; });
}

uint64_t Simulation::steps()
{
    return lua.execute<uint64_t>([this](lua_State*) { return steps_; });
}
