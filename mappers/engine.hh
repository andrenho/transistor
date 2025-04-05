#ifndef ENGINE_HH
#define ENGINE_HH

#include <optional>

#include "in/compiled_circuit.hh"
#include "in/render.hh"
#include "luaenv/lua.hh"
#include "out/event.hh"

class Engine {
public:
    explicit Engine(Lua& lua);

    void setup();
    void save_in_progress();

    std::optional<luaobj::CompiledCircuit> events(std::vector<luaobj::Event> const& events);

    [[nodiscard]] luaobj::Render render(std::unordered_map<uint32_t, uint8_t> const& wires_values) const;

    [[nodiscard]] bool                              compilation_successful() const { return compilation_successful_; }
    [[nodiscard]] std::string const&                compilation_messages() const { return compilation_messages_; }
    [[nodiscard]] std::optional<std::string> const& test_errors() const { return test_errors_; }


private:
    Lua& lua_;

    void init();
    void setup_dev();
    void setup_release();

    static std::pair<bool, std::string> check_engine();
    std::pair<bool, std::string>        run_tests() const;

    bool                       compilation_successful_ = true;
    std::string                compilation_messages_;
    std::optional<std::string> test_errors_;
};

#endif //ENGINE_HH
