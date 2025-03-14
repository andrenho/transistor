#ifndef ENGINE_HH
#define ENGINE_HH

#include <functional>
#include <cstdint>
#include <lua.hpp>

#include "snapshot.hh"
#include "simulation/simulation.hh"

class Engine {
public:
    enum class Button { Left, Middle, Right };

    Engine();
    ~Engine();

    void start();

    Snapshot take_snapshot();

    void cursor_set_pointer(BoardId board_id, int x, int y, Direction dir);
    void cursor_click(BoardId board_id, Button button);
    void cursor_button_release(BoardId board_id, Button button);
    void cursor_key_press(BoardId board_id, char key);
    void cursor_key_release(BoardId board_id, char key);
    void cursor_select_component_def(BoardId board_id, std::string const& key);

    void render_all_components(BoardId board_id, int G_luaref, int tile_size);

    template <typename F>
    void with_lua_object(F f) {
        simulation_.pause();
        f(L);
        simulation_.resume();
    }

private:
    lua_State* L;
    Simulation simulation_;
    int sandbox_ref_ = -1;

    void register_load_all_components_function() const;

    void load_bytecode(const char* name, uint8_t const* bytecode, size_t sz) const;
    void execute(std::string const& command, bool recompile, std::function<void()> const& and_also_do=nullptr);

    void recompile_sandbox();

    friend class Tests;
};

#endif //ENGINE_HH
