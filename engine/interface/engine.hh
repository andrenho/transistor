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

    Simulation& simulation() { return simulation_; }

private:
    Simulation simulation_;
    int sandbox_ref_ = -1;

    void register_load_all_components_function(lua_State* L) const;

    void load_bytecode(lua_State* L, const char* name, uint8_t const* bytecode, size_t sz) const;
    void execute(std::string const& command, bool recompile, std::function<void(lua_State*)> const& and_also_do=nullptr);

    void recompile_sandbox(lua_State* L);

    friend class Tests;
};

#endif //ENGINE_HH
