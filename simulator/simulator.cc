#include "simulator.hh"

#include <chrono>
#include <string>
using namespace std::string_literals;
using namespace std::chrono_literals;

Simulator::Simulator(Lua& lua)
    : lua_(lua),
      thread_([this] { main_thread(); }),
      simulate_luaref_(lua.with_lua<int>([this](lua_State* L) {
         lua_getglobal(L, "simulate");
         return luaL_ref(L, LUA_REGISTRYINDEX);
      }))
{
}

Simulator::~Simulator()
{
    running_ = false;
    thread_.join();
}

void Simulator::main_thread()
{
    while (running_) {
        single_step();

        // give the CPU a break
        switch (cpu_usage_) {
            case CpuUsage::Light:      std::this_thread::sleep_for(100ns); break;
            case CpuUsage::Normal:     std::this_thread::yield(); break;
            case CpuUsage::Aggressive: break;
        }
    }
}

void Simulator::single_step()   // here's where the magic happens
{
    std::lock_guard lock_guard(mutex_);

    // simulate components (C)
    for (auto& component: circuit_.components)
        if (component.simulate)
            component.simulate(component.data->data, component.pins->data);

    // simulate components
    lua_.with_lua([this](lua_State* L) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, simulate_luaref_);
        if (lua_pcall(L, 0, 0, 0) != LUA_OK)
            throw std::runtime_error("Error executing Lua simulation: "s + lua_tostring(L, -1));
    });

    // update connection and pin values
    for (auto& connection: circuit_.connections) {

        // calculate value from output pins
        connection.value = 0;
        for (auto const& pin: connection.pins)
            if (pin.dir == luaobj::PinDirection::Output)
                connection.value |= *pin.pin_value_ptr;

        // set input pins
        for (auto const& pin: connection.pins)
            if (pin.dir == luaobj::PinDirection::Input)
                *pin.pin_value_ptr = connection.value;
    }

    ++steps_;
}

void Simulator::update_compiled_circuit(luaobj::CompiledCircuit&& circuit)
{
    std::lock_guard lock_guard(mutex_);
    circuit_ = std::move(circuit);
}

void Simulator::update_cpu_usage(CpuUsage usage)
{
    std::lock_guard lock_guard(mutex_);
    cpu_usage_ = usage;
}

uint64_t Simulator::steps()
{
    std::lock_guard lock_guard(mutex_);
    uint64_t s = steps_;
    steps_ = 0;
    return s;
}

std::unordered_map<uint32_t, uint8_t> Simulator::wires_values() const
{
    std::lock_guard lock_guard(mutex_);

    std::unordered_map<uint32_t, uint8_t> r;
    r.reserve(circuit_.total_wires);
    for (auto const& connection: circuit_.connections)
        for (auto const& wire_hash: connection.wire_pos_hashes)
            r[wire_hash] = connection.value;
    return r;
}
