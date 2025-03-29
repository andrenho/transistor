#ifndef ENGINE_HH
#define ENGINE_HH

#include "in/render.hh"
#include "luaenv/lua.hh"
#include "out/event.hh"

class Engine {
public:
    explicit Engine(Lua& lua);

    void setup();
    void events(std::vector<Event> const& events);

    [[nodiscard]] Render render() const;

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
