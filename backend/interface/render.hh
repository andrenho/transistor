#ifndef RENDER_HH
#define RENDER_HH

#include <lua.hpp>
#include <optional>
#include <string>
#include <variant>
#include <vector>

struct EngineCompilation {
    bool                       success = true;
    std::string                compilation_messages;
    std::optional<std::string> test_errors;
};

struct Render {
    EngineCompilation  engine_compilation;
    int                steps_per_second = 0;

    void load_from_lua(lua_State* L);
};

#endif //RENDER_HH
