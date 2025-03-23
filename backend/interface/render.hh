#ifndef RENDER_HH
#define RENDER_HH

#include <lua.hpp>
#include <optional>
#include <string>
#include <variant>
#include <vector>

template<typename ... Ts>
struct overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> overload(Ts...) -> overload<Ts...>;

struct DrawInstruction {
    std::string image;
    int x, y, w, h;
};

using Instruction = std::variant<DrawInstruction>;

using Scene = std::vector<Instruction>;

struct EngineCompilation {
    bool                       success = true;
    std::string                compilation_messages;
    std::optional<std::string> test_errors;
};

struct Render {
    std::vector<Scene> scenes;
    EngineCompilation  engine_compilation;
    int                steps_per_second = 0;

    void load_from_lua(lua_State* L);

private:
    void add_instruction(lua_State* L, Scene& scene);
};

#endif //RENDER_HH
