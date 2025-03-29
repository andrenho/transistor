#ifndef ENGINE_HH
#define ENGINE_HH

#include "luaenv/lua.hh"

class Engine {
public:
    explicit Engine(Lua& lua);

    void setup();

private:
    Lua& lua_;

    void init();
    void setup_dev();
    void setup_release();

    static std::pair<bool, std::string> check_engine();
    std::pair<bool, std::string>        run_tests() const;

    bool                       compilation_successful = true;
    std::string                compilation_messages;
    std::optional<std::string> test_errors;
};

#endif //ENGINE_HH
