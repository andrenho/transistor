#ifndef RENDER_HH
#define RENDER_HH

#include <optional>
#include <string>

struct EngineCompilation {
    bool                       success = true;
    std::string                compilation_messages;
    std::optional<std::string> test_errors;
};

struct Render {
    EngineCompilation engine_compilation;

    int steps_per_second = 0;
};

#endif //RENDER_HH
