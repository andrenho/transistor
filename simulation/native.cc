#include "native.hh"

void sim_button(uint8_t* data, uint8_t* pins)
{
    data[0] = 255;
}

std::unordered_map<std::string, SimFunc> native_functions = {
    { "__button", sim_button },
};
