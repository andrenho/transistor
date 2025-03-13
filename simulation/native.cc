#include "native.hh"

void sim_button(uint8_t* data, uint8_t* pins)
{
    pins[0] = pins[1] = pins[2] = pins[3] = data[0];
}

std::unordered_map<std::string, SimFunc> native_functions = {
    { "__button", sim_button },
};
