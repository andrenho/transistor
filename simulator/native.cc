#include "native.hh"

std::unordered_map<std::string, SimFunc> native_functions = {

    // BASIC COMPONENTS

    { "__button", [](uint8_t* data, uint8_t* pins) {
        pins[0] = pins[1] = pins[2] = pins[3] = data[0];
    } },
    { "__led", [](uint8_t* data, uint8_t* pins) {
        data[0] = pins[0] | pins[1] | pins[2] | pins[3];
    } },
    { "__vcc", [](uint8_t* data, uint8_t* pins) {
        pins[0] = pins[1] = pins[2] = pins[3] = 1;
    } },
    { "__npn", [](uint8_t* data, uint8_t* pins) {
        constexpr int SW0 = 0, SW1 = 2, IN = 1, OUT = 3;
        pins[OUT] = pins[IN] & (pins[SW0] | pins[SW1]);
    } },
    { "__pnp", [](uint8_t* data, uint8_t* pins) {
        constexpr int SW0 = 0, SW1 = 2, IN = 1, OUT = 3;
        pins[OUT] = pins[IN] & !(pins[SW0] | pins[SW1]);
    } },

    // GATES

    { "__or_2i", [](uint8_t* data, uint8_t* pins) {
        constexpr int IN0 = 0, IN1 = 1, Q_ = 2, Q = 3;
        pins[Q] = pins[IN0] | pins[IN1];
        pins[Q_] = !pins[Q];
    } },
};
