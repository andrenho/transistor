#ifndef NATIVE_HH
#define NATIVE_HH

#include <string>
#include <unordered_map>

typedef void(*SimFunc)(uint8_t* data, uint8_t* pins);

extern std::unordered_map<std::string, SimFunc> native_functions;

#endif //NATIVE_HH
